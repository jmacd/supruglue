// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"
#include "lib/coroutine/coroutine.h"
#include "lib/gpio/gpio.h"
#include "lib/log/journal/journal.h"
#include "lib/time/clock.h"

// Per-bit half-period. The protocol is slow and timing-tolerant; ~1ms per
// clock phase (power off, then on) is well within spec.
#define UI1203_BIT_DELAY (TIME_SECOND / 1000)

// Continuous power required before the meter begins transmitting.
#define UI1203_WARMUP (3 * TIME_SECOND)

// Interval between successive whole-message reads.
#define UI1203_READ_INTERVAL (10 * TIME_SECOND)

// Upper bound on bits read while searching for frame alignment: a few message
// lengths' worth, so a silent or unsynchronizable line does not loop forever.
#define UI1203_SYNC_MAX_BITS (UI1203_MSG_MAX * 10 * 2)

// powerUp applies continuous power so the meter resets and begins transmitting.
static void powerUp(UI1203_Reader *rdr) {
  GPIO_SetPin(rdr->clock_out, 1);
  Sleep32(UI1203_WARMUP);
}

// powerDown removes power from the meter.
static void powerDown(UI1203_Reader *rdr) {
  GPIO_SetPin(rdr->clock_out, 0);
}

// readBit clocks out one bit by cycling power (off, then on) and samples the
// data line. The data line is open-collector and active LOW, so a LOW level is
// a 1 bit.
static int readBit(UI1203_Reader *rdr) {
  GPIO_SetPin(rdr->clock_out, 0);
  Sleep32(UI1203_BIT_DELAY);
  GPIO_SetPin(rdr->clock_out, 1);
  Sleep32(UI1203_BIT_DELAY);

  return GPIO_GetPin(rdr->data_in) == 0 ? 1 : 0;
}

// syncByte slides a 10-bit window one bit at a time until a valid frame is
// found, establishing byte alignment. Returns the decoded ASCII value, or -1 if
// no valid frame appears within UI1203_SYNC_MAX_BITS.
static int32_t syncByte(UI1203_Reader *rdr) {
  uint16_t window = 0;
  uint32_t i;

  for (i = 0; i < UI1203_SYNC_MAX_BITS; i++) {
    int bit = readBit(rdr);
    window = (uint16_t)((window >> 1) | (bit << 9));

    int32_t data = bitsToAscii(window);
    if (data >= 0) {
      return data;
    }
  }
  return -1;
}

// readByte reads exactly one frame-aligned byte (10 bits) and decodes it.
// Returns the ASCII value, or -1 on a framing/parity error.
static int32_t readByte(UI1203_Reader *rdr) {
  uint16_t window = 0;
  int      b;

  for (b = 0; b < 10; b++) {
    int bit = readBit(rdr);
    window = (uint16_t)((window >> 1) | (bit << 9));
  }
  return bitsToAscii(window);
}

SUPRUGLUE_DEFINE_THREAD(ui1203reader, 256);

void readerRunner(ThreadID tid, Args args) {
  UI1203_Reader *rdr = (UI1203_Reader *)args.ptr;
  Timestamp      clock;
  ReadClock(&clock);

  while (1) {
    powerUp(rdr);

    rdr->length = 0;

    // Synchronize to the first frame, then read aligned bytes until the
    // terminating carriage return or the message buffer fills.
    int32_t data = syncByte(rdr);
    while (data >= 0 && rdr->length < UI1203_MSG_MAX) {
      rdr->message[rdr->length++] = (char)data;
      if (data == '\r') {
        break;
      }
      data = readByte(rdr);
    }

    if (data < 0) {
      PRULOG_1u32(WARNING, "ui1203 framing error after %u bytes", rdr->length);
    } else {
      PRULOG_1u32(INFO, "ui1203 read %u bytes", rdr->length);
    }

    // TODO(M2): forward rdr->message to the host over RPMsg.

    powerDown(rdr);
    SleepUntil32(&clock, UI1203_READ_INTERVAL);
  }
}

void UI1203_Init_Reader(UI1203_Reader *rdr, gpio_pin clock, gpio_pin data) {
  rdr->clock_out = clock;
  rdr->data_in = data;
  rdr->length = 0;

  Args args;
  args.ptr = (const char *)rdr;

  Create(&ui1203reader.thread, readerRunner, args, "ui1203reader", sizeof(ui1203reader.space));
}

// Bits:
// 0: start
// 1-7: data (LSB first)
// 8: parity
// 9: stop

int32_t asciiToBits(int32_t c) {
  int32_t ci = (uint32_t)c;
  int32_t d0 = ci << 1;
  int32_t x1 = (d0 >> 4) ^ d0;
  int32_t x2 = (x1 >> 2) ^ x1;
  int32_t x3 = (x2 >> 1) ^ x2;
  int32_t parity = ~x3 & 0x1;

  d0 |= parity << 8;
  d0 |= 1 << 9;
  return d0;
}

int32_t bitsToAscii(int32_t bits) {
  int32_t  x1 = (bits >> 4) ^ bits;
  int32_t  x2 = (x1 >> 2) ^ x1;
  int32_t  x3 = (x2 >> 1) ^ x2;
  uint16_t parity = (~x3 & 1);
  uint16_t calc = (bits & 0x100) >> 8;

  if (parity != calc || (bits & 0x201) != 0x200) {
    return -1;
  }

  return (bits >> 1) & 0x7f;
}
