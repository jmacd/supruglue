// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"

#include "lib/coroutine/coroutine.h"
#include "lib/gpio/gpio.h"
#include "lib/gpio/test32/gpio.h"
#include "lib/intc/service.h"
#include "lib/log/daemon/daemon.h"
#include "lib/log/journal/journal.h"
#include "lib/pinmap/pinmap.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/process.h"
#include "gtest/gtest.h"

#include <cstring>
#include <string>
#include <thread>
#include <vector>

// asciiToBits/bitsToAscii round-trip for all 7-bit values.
TEST(Ui1203Test, Bits) {
  for (unsigned char c = 0; c < 128; c++) {
    int32_t bits = asciiToBits(c);
    uint8_t ascii = bitsToAscii(bits);

    EXPECT_EQ(c, ascii);
  }
}

// Build the on-wire bit stream (start..stop, LSB first) for a string.
static std::vector<int> frameBits(const std::string &s) {
  std::vector<int> bits;
  for (char c : s) {
    int32_t f = asciiToBits((unsigned char)c);
    for (int b = 0; b < 10; b++) {
      bits.push_back((f >> b) & 1);
    }
  }
  return bits;
}

// Feed every bit through the decoder, returning decoded bytes and counting the
// parity and framing errors reported.
static std::string decodeAll(const std::vector<int> &bits, int *parity = nullptr, int *framing = nullptr) {
  UI1203_Decoder dec;
  UI1203_DecoderInit(&dec);

  std::string out;
  int         perr = 0;
  int         ferr = 0;
  for (int bit : bits) {
    int32_t byte = 0;
    int     r = UI1203_FeedBit(&dec, bit, &byte);
    if (r == UI1203_BYTE) {
      out.push_back((char)byte);
    } else if (r == UI1203_PARITY_ERROR) {
      perr++;
    } else if (r == UI1203_FRAME_ERROR) {
      ferr++;
    }
  }
  if (parity != nullptr) {
    *parity = perr;
  }
  if (framing != nullptr) {
    *framing = ferr;
  }
  return out;
}

// A frame-aligned stream decodes exactly with no errors.
TEST(Ui1203Decoder, Aligned) {
  int         parity = -1, framing = -1;
  std::string got = decodeAll(frameBits("R12\r"), &parity, &framing);
  EXPECT_EQ("R12\r", got);
  EXPECT_EQ(0, parity);
  EXPECT_EQ(0, framing);
}

// Leading idle (mark) bits do not false-sync; the decoder still recovers the
// message once frames begin.
TEST(Ui1203Decoder, MisalignedResync) {
  std::vector<int> bits;
  for (int i = 0; i < 7; i++) {
    bits.push_back(1); // idle mark; bit0 != start, never a valid frame
  }
  std::vector<int> frames = frameBits("R12\r");
  bits.insert(bits.end(), frames.begin(), frames.end());

  std::string got = decodeAll(bits);
  EXPECT_NE(std::string::npos, got.find("R12\r"));
}

// A flipped parity bit on an aligned frame is reported as a parity error (not a
// framing error), and the decoder stays aligned to decode the next frame.
TEST(Ui1203Decoder, ParityError) {
  std::vector<int> bits = frameBits("R1\r");
  bits[10 + 8] ^= 1; // parity bit of the second frame ('1')

  int         parity = 0, framing = 0;
  std::string got = decodeAll(bits, &parity, &framing);
  EXPECT_EQ(1, parity);
  EXPECT_EQ(0, framing);
  EXPECT_NE(std::string::npos, got.find('R'));
  EXPECT_NE(std::string::npos, got.find('\r'));
}

// A flipped stop bit on an aligned frame is reported as a framing error.
TEST(Ui1203Decoder, FramingError) {
  std::vector<int> bits = frameBits("R1\r");
  bits[10 + 9] ^= 1; // stop bit of the second frame ('1')

  int parity = 0, framing = 0;
  decodeAll(bits, &parity, &framing);
  EXPECT_EQ(0, parity);
  EXPECT_EQ(1, framing);
}

// End-to-end: a simulated meter drives the data line in response to the reader's
// clock toggles; the real reader thread decodes the reading and its events reach
// the host over RPMsg as journal log entries delivered by the syslog daemon.

struct FakeMeter {
  gpio_pin         clock;
  gpio_pin         data;
  std::vector<int> bits;
  size_t           idx = 0;

  // On each falling clock edge the meter advances to the next bit and presents
  // it on the open-collector (active LOW) data line.
  void onFallingEdge() {
    int b = bits[idx];
    idx = (idx + 1) % bits.size();
    GPIO_SetPin(data, b ? 0 : 1);
  }

  static void hook(void *ctx, gpio_bank *bank, int reg, uint32_t value) {
    FakeMeter *m = (FakeMeter *)ctx;
    if (reg == GPIOREG_CLEARDATAOUT && bank == m->clock.bank && (value & (1u << m->clock.bit)) != 0) {
      m->onFallingEdge();
    }
  }
};

UI1203_Reader e2e_reader;

// runReader drives the given meter bit stream through the real reader and
// collects the log entries the host receives, until an entry whose format
// string contains needle arrives (returning that entry) or a cap is hit.
static bool runReader(FakeMeter *meter, const char *needle, Entry *found) {
  GPIO_TestReset();
  GPIO_TestSetWriteHook(FakeMeter::hook, meter);

  auto tt = NewTestTransport();

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, InterruptServiceInit());
  ClockInit();
  GPIO_Init();
  SyslogInit();

  UI1203_Init_Reader(&e2e_reader, meter->clock, meter->data);

  bool  ok = false;
  Entry hit;
  std::thread client([tt, needle, &ok, &hit] {
    for (int i = 0; i < 200; i++) {
      Entry    entry;
      uint16_t blen = sizeof(entry);
      if (HostRecv(tt, &entry, &blen) != 0) {
        break;
      }
      if (entry.msg != nullptr && std::strstr(entry.msg, needle) != nullptr) {
        hit = entry;
        ok = true;
        break;
      }
    }
    Shutdown();
  });

  EXPECT_EQ(0, ::Run());
  client.join();

  if (ok) {
    *found = hit;
  }
  return ok;
}

TEST(Ui1203EndToEnd, ReadingOverRpmsg) {
  FakeMeter meter;
  meter.clock = GPIO_PIN(P9_25);
  meter.data = GPIO_PIN(P9_23);
  meter.bits = frameBits("R1234\r");

  Entry found;
  ASSERT_TRUE(runReader(&meter, "ui1203 reading", &found));
  EXPECT_EQ(1234u, found.int1.U64);
  EXPECT_EQ(0u, found.int2.U64); // no errors
}

TEST(Ui1203EndToEnd, ParityErrorOverRpmsg) {
  std::vector<int> bits = frameBits("R12\r");
  bits[10 + 8] ^= 1; // corrupt the parity of the '1' frame

  FakeMeter meter;
  meter.clock = GPIO_PIN(P9_25);
  meter.data = GPIO_PIN(P9_23);
  meter.bits = bits;

  Entry found;
  ASSERT_TRUE(runReader(&meter, "parity error", &found));
}
