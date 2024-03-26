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
  SemaUp(&rdr->lock);
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
    uint16_t parity = 0;
    uint16_t count = 0;

    PWM_Enable();

    for (; position < 32;) {
      // V;RBxxxxxxx;IByyyyy;Kmmmmm\r
      // or
      // Rxxxxyyyyyyyy\r

      // Note that ASCII:
      // '0' is 0x30 (i.e., decimal 48)
      // 'R' is 0x52 (i.e., decimal 82)
      // '\r` is 0xd (i.e., decimal 13)

      for (;; count++) {

        SemaDown(&rdr->lock);

        int bit = GPIO_GetPin(rdr->data_in);

        parity = (parity ^ bit ^ byte) & 0x1;
        byte = (byte >> 1) | (bit << 9);

        // Bits:
        // 0: start
        // 1-7: data (LSB first)
        // 8: parity
        // 9: stop

        // When parity matches and the start and stop bits are correct.
        if (parity == 1 && count >= 10 && (byte & 0x201) == 0x200) {
          // output a byte
          PRULOG_1u32(INFO, "ui1203 byte 0x%x", (byte >> 1) & 0x7f);

          if (position != 0 && count > 10) {
            PRULOG_1u32(INFO, "ui1203 unused bits: %u", count);
            break;
          }
          position++;
          count = 0;
          byte = 0;
          parity = 0;
        }
      }
    }

    PWM_Disable();

    SleepUntil32(&clock, 10 * TIME_SECOND);
  }
}

SUPRUGLUE_DEFINE_THREAD(ui1203reader, 256);
SUPRUGLUE_DEFINE_THREAD(ui1203writer, 256);

void UI1203_Init_Reader(UI1203_Reader *rd, gpio_pin data_pin) {
  rd->data_in = data_pin;

  PWM_Init(1500, 3000, 2000);

  Args args; // @@@
  args.ptr = (const char *)rd;

  InterruptHandlerInit(SYSEVT_TPCC_INT_PEND_PO1, readerHandler, args);

  Create(&ui1203reader.thread, readerRunner, args, "ui1203reader", sizeof(ui1203reader.space));
}

void writerHandler(Args args) {
  // Called after rising edge of clock, produces one bit via GPIO.
  // @@@ Keep state about current position
  UI1203_Writer *wr = (UI1203_Writer *)args.ptr;

  GPIO_SetPin(wr->data_out, 0); // @@@
}

void writerRunner(ThreadID tid, Args args) {
  Sleep32(TIME_SECOND);
}

void UI1203_Init_Writer(UI1203_Writer *wr, gpio_pin data_pin) {
  wr->data_out = data_pin;

  CAP_Init();

  Args args; // @@@
  args.ptr = (const char *)wr;

  InterruptHandlerInit(SYSEVT_PR1_PRU_ECAP_INTR_REQ, writerHandler, args);

  Create(&ui1203writer.thread, writerRunner, args, "ui1203writer", sizeof(ui1203writer.space));
}
