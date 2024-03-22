// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"
#include <stdio.h>

// @@@ Need to pass void* address through INTC?
// (Anyway not clear how multiple PWM or CAP units will be
// configured or compiled.)

void readerHandler(void) {
  // Called after waiting for settlement period, receives one
  // bit via GPIO.

  Sema_Up();
}

// PERIOD is 10 seconds
#define PERIOD 2000000000

void readerRunner(ThreadID tid, Args args) {
  // @@@ Create thread, give it measurement interval.  It will
  // Sleep, then start clock, read, stop clock, Sleep, ...

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    // Reset state

    PWM_Enable();

    for (int p = 0; p < 32; p++) {
      // Bytes
      uint16_t byte = 0;
      for (int b = 0; b < 10; b++;) {
        // Bits
        Sema_Down();
        byte |= GPIO_GetPin(pin) << b;
      }
      rd->buffer[p] = byte;
    }

    PWM_Disable();

    SleepUntil(&clock, BLUE_PERIOD);
  }
}

SUPRUGLUE_DEFINE_THREAD(ui1203reader, 256);

void UI1203_Init_Reader(UI1203_Reader *rd, gpio_pin data) {
  rd->data = data;

  PWM_Init(1500, 3000, 2000);

  InterruptHandlerInit(SYSEVT_TPCC_INT_PEND_PO1, readerHandler);

  Create(&blue.thread, toggle_blue, args, "blue", sizeof(blue.space));
}

void writerHandler(void) {
  // Called after rising edge of clock, produces one bit via GPIO.
  // @@@ Keep state about current position

  GPIO_SetPin(pin, X);
}

void UI1203_Init_Writer(UI1203_Writer *wr, gpio_pin data) {
  wr->data = data;

  CAP_Init();

  InterruptHandlerInit(SYSEVT_PR1_PRU_ECAP_INTR_REQ, writerHandler);
}
