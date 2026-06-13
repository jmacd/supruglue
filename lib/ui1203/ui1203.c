// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"
#include "lib/coroutine/coroutine.h"
#include "lib/gpio/gpio.h"
#include "lib/log/journal/journal.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/clock.h"

// Per-bit half-period. The protocol is slow and timing-tolerant; ~1ms per
// clock phase (power off, then on) is well within spec.
#define UI1203_BIT_DELAY (TIME_SECOND / 1000)

// Continuous power required before the meter begins transmitting.
#define UI1203_WARMUP (3 * TIME_SECOND)

// Interval between successive whole-message reads.
#define UI1203_READ_INTERVAL (10 * TIME_SECOND)

// Upper bound on bits read for one message, generous enough to absorb the
// initial frame search plus a full reading, so a silent or unsynchronizable
// line does not loop forever.
#define UI1203_MAX_BITS (UI1203_MSG_MAX * 10 * 3)

void UI1203_DecoderInit(UI1203_Decoder *dec) {
  dec->window = 0;
  dec->synced = 0;
  dec->count = 0;
  dec->fill = 0;
}

int UI1203_FeedBit(UI1203_Decoder *dec, int bit, int32_t *out) {
  dec->window = (uint16_t)((dec->window >> 1) | ((bit & 1) << 9));

  if (!dec->synced) {
    // Wait until the window holds ten real bits before attempting to lock, so a
    // partially filled window cannot produce a false frame match.
    if (dec->fill < 10) {
      dec->fill++;
      if (dec->fill < 10) {
        return UI1203_NEED_MORE;
      }
    }

    int32_t v = bitsToAscii(dec->window);
    if (v < 0) {
      return UI1203_NEED_MORE;
    }
    dec->synced = 1;
    dec->count = 0;
    *out = v;
    return UI1203_BYTE;
  }

  // Aligned: a frame completes every 10 bits.
  if (++dec->count < 10) {
    return UI1203_NEED_MORE;
  }
  dec->count = 0;

  int32_t v = bitsToAscii(dec->window);
  if (v >= 0) {
    *out = v;
    return UI1203_BYTE;
  }

  // A single frame failed validation. The meter transmits contiguous,
  // frame-aligned bytes, so this is most likely a transient bit error: surface
  // it but stay aligned so the next frame still decodes.
  return UI1203_FRAME_ERROR;
}

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

// readMessage reads one whole reading into rdr->message, returning the number
// of bytes (including the terminating carriage return), or 0 on timeout.
static uint32_t readMessage(UI1203_Reader *rdr) {
  UI1203_Decoder dec;
  uint32_t       i;

  UI1203_DecoderInit(&dec);
  rdr->length = 0;

  for (i = 0; i < UI1203_MAX_BITS && rdr->length < UI1203_MSG_MAX; i++) {
    int     bit = readBit(rdr);
    int32_t byte;

    int r = UI1203_FeedBit(&dec, bit, &byte);
    if (r == UI1203_FRAME_ERROR) {
      PRULOG_0(WARNING, "ui1203 framing error");
      continue;
    }
    if (r != UI1203_BYTE) {
      continue;
    }

    rdr->message[rdr->length++] = (char)byte;
    if (byte == '\r') {
      return rdr->length;
    }
  }
  return 0;
}

// The larger stack is needed for host (test32) testing, where native stack
// frames are bigger than the compact clpru ones; on the PRU a 256 byte stack
// is sufficient and DMEM is tight.
#if defined(SUPRUGLUE_TEST32)
SUPRUGLUE_DEFINE_THREAD(ui1203reader, 512);
#else
SUPRUGLUE_DEFINE_THREAD(ui1203reader, 256);
#endif

void readerRunner(ThreadID tid, Args args) {
  UI1203_Reader *rdr = (UI1203_Reader *)args.ptr;
  Timestamp      clock;
  ReadClock(&clock);

  while (1) {
    powerUp(rdr);

    uint32_t len = readMessage(rdr);
    if (len == 0) {
      PRULOG_0(WARNING, "ui1203 read timed out");
    } else {
      PRULOG_1u32(INFO, "ui1203 read %u bytes", len);

      // Forward the reading to the host, retrying until it is accepted.
      while (ClientSend(&__transport, rdr->message, (uint16_t)len) != 0) {
        Sleep32(TIME_SECOND / 2);
      }
    }

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
