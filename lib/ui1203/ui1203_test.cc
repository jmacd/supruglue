// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"

#include "lib/coroutine/coroutine.h"
#include "lib/gpio/gpio.h"
#include "lib/gpio/test32/gpio.h"
#include "lib/intc/service.h"
#include "lib/pinmap/pinmap.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/process.h"
#include "gtest/gtest.h"

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

// Feed every bit through the decoder, returning decoded bytes and the number of
// framing errors reported.
static std::string decodeAll(const std::vector<int> &bits, int *errors = nullptr) {
  UI1203_Decoder dec;
  UI1203_DecoderInit(&dec);

  std::string out;
  int         errs = 0;
  for (int bit : bits) {
    int32_t byte = 0;
    int     r = UI1203_FeedBit(&dec, bit, &byte);
    if (r == UI1203_BYTE) {
      out.push_back((char)byte);
    } else if (r == UI1203_FRAME_ERROR) {
      errs++;
    }
  }
  if (errors != nullptr) {
    *errors = errs;
  }
  return out;
}

// A frame-aligned stream decodes exactly.
TEST(Ui1203Decoder, Aligned) {
  int         errors = -1;
  std::string got = decodeAll(frameBits("R12\r"), &errors);
  EXPECT_EQ("R12\r", got);
  EXPECT_EQ(0, errors);
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

// A corrupted frame is reported as an error, and the decoder re-syncs on the
// following frames.
TEST(Ui1203Decoder, FrameErrorAndRecover) {
  std::vector<int> bits = frameBits("RX\r");
  // Flip the parity bit (index 8) of the second frame ('X').
  bits[10 + 8] ^= 1;

  int         errors = 0;
  std::string got = decodeAll(bits, &errors);
  EXPECT_GE(errors, 1);
  EXPECT_NE(std::string::npos, got.find('R'));
  EXPECT_NE(std::string::npos, got.find('\r'));
}

// End-to-end: a simulated meter drives the data line in response to the reader's
// clock toggles, and the real reader thread decodes the reading and delivers it
// to the host over RPMsg.

static const char *const kMeterReading = "R1234\r";

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

TEST(Ui1203EndToEnd, ReadsReading) {
  GPIO_TestReset();

  gpio_pin clock = GPIO_PIN(P9_25);
  gpio_pin data = GPIO_PIN(P9_23);

  FakeMeter meter;
  meter.clock = clock;
  meter.data = data;
  meter.bits = frameBits(kMeterReading);
  GPIO_TestSetWriteHook(FakeMeter::hook, &meter);

  auto tt = NewTestTransport();

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, InterruptServiceInit());
  ClockInit();
  GPIO_Init();

  UI1203_Init_Reader(&e2e_reader, clock, data);

  std::string received;
  std::thread client([tt, &received] {
    char     buf[64];
    uint16_t blen = sizeof(buf);
    EXPECT_EQ(0, HostRecv(tt, buf, &blen));
    received.assign(buf, blen);
    Shutdown();
  });

  EXPECT_EQ(0, ::Run());
  client.join();

  EXPECT_EQ(kMeterReading, received);
}
