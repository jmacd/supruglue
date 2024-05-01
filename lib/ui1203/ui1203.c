// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"
#include "lib/cap/cap.h"
#include "lib/coroutine/coroutine.h"
#include "lib/gpio/gpio.h"
#include "lib/intc/intc.h"
#include "lib/log/journal/journal.h"
#include "lib/pwm/pwm.h"
#include "lib/soc/sysevts.h"
#include "lib/sync/sync.h"
#include "lib/time/clock.h"
#include <stdio.h>

void readerHandler(Args args) {
  UI1203_Reader *rdr = (UI1203_Reader *)args.ptr;
  SemaphoreUp(&rdr->sem);

  // PRULOG_0(INFO_NOYIELD, "pwm interrupt");

  PWM_ClearInterrupt();
}

void readerRunner(ThreadID tid, Args args) {
  UI1203_Reader *rdr = (UI1203_Reader *)args.ptr;
  Timestamp      clock;
  ReadClock(&clock);

  while (1) {
    // Reset state
    uint16_t position = 0;
    uint16_t byte = 0;
    uint16_t count = 0;

    // PWM_Enable();

    for (; position < 32;) {
      // V;RBxxxxxxx;IByyyyy;Kmmmmm\r
      // or
      // Rxxxxyyyyyyyy\r

      // Note that ASCII:
      // '0' is 0x30 (i.e., decimal 48)
      // 'R' is 0x52 (i.e., decimal 82)
      // '\r` is 0xd (i.e., decimal 13)

      for (;; count++) {
        SemaphoreDown(&rdr->sem);

        int bit = GPIO_GetPin(rdr->data_in);

        // PRULOG_1u32(INFO, "ui1203 read bit 0x%x", bit);
        byte = (byte >> 1) | (bit << 9);

        int32_t data = bitsToAscii(byte);

        // PRULOG_2u32(INFO, "ui1203 read byte 0x%x %u", byte, parity);

        // When parity matches and the start and stop bits are correct.
        if (data >= 0) {
          // output a byte
          PRULOG_1u32(INFO, "ui1203 byte 0x%x", data);

          if (position != 0 && count > 10) {
            // PRULOG_1u32(INFO, "ui1203 unused bits: %u", count);
            break;
          }
          position++;
          count = 0;
          byte = 0;
        }
      }
    }

    // PWM_Disable();

    SleepUntil32(&clock, 10 * TIME_SECOND);
  }
}

SUPRUGLUE_DEFINE_THREAD(ui1203reader, 256);
SUPRUGLUE_DEFINE_THREAD(ui1203writer, 256);

void UI1203_Init_Reader(UI1203_Reader *rd, gpio_pin data_pin) {
  SemaphoreInit(&rd->sem);
  rd->data_in = data_pin;

  Args args; // @@@
  args.ptr = (const char *)rd;

  InterruptHandlerInit(SYSEVT_TPCC_INT_PEND_PO1, readerHandler, args);

  Create(&ui1203reader.thread, readerRunner, args, "ui1203reader", sizeof(ui1203reader.space));
}

void writerHandler(Args args) {
  // Called after rising edge of clock, produces one bit via GPIO.
  UI1203_Writer *wr = (UI1203_Writer *)args.ptr;
  SemaphoreUp(&wr->sem);
  CAP_ClearInterrupt();
}

void writerRunner(ThreadID tid, Args args) {
  UI1203_Writer *wr = (UI1203_Writer *)args.ptr;

  while (1) {

    const char *p = "R";

    for (; *p != 0; p++) {

      int32_t d10 = asciiToBits(*p);

      PRULOG_1u32(INFO, "write word is 0x%x", d10);

      int b;
      for (b = 0; b < 10; b++) {
        SemaphoreDown(&wr->sem);

        int32_t bit = d10 & 1;
        PRULOG_2u32(INFO, "write bit %d is %d", b, bit);
        GPIO_SetPin(wr->data_out, bit);

        d10 >>= 1;
      }
    }
  }
}

void UI1203_Init_Writer(UI1203_Writer *wr, gpio_pin data_pin) {
  SemaphoreInit(&wr->sem);

  wr->data_out = data_pin;

  Args args; // @@@
  args.ptr = (const char *)wr;

  InterruptHandlerInit(SYSEVT_PR1_PRU_ECAP_INTR_REQ, writerHandler, args);

  Create(&ui1203writer.thread, writerRunner, args, "ui1203writer", sizeof(ui1203writer.space));
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
