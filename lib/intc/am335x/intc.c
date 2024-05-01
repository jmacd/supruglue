// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_intc.h"
#include "lib/debug/debug.h"
#include "lib/intc/am335x/intc.h"
#include "lib/soc/sysevts.h"

volatile register uint32_t __R31;

InterruptController __controller;

#if SUPRUGLUE_PRU_NUM == 0
#define ARM_TO_PRU_EVT SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ
#define ARM_TO_PRU_IRQ PRU_HOST0_INTERRUPT
#define HIPRIO_EVT CT_INTC.HIPIR0
#else
#define ARM_TO_PRU_EVT SYSEVT_PR1_PRU_MST_INTR3_INTR_REQ
#define ARM_TO_PRU_IRQ PRU_HOST1_INTERRUPT
#define HIPRIO_EVT CT_INTC.HIPIR1
#endif

void ControllerInit(void) {
  uint8_t evt;
  for (evt = 0; evt < NUM_SYSEVTS; evt++) {
    __controller.handler[evt] = NULL;
    __controller.args[evt].ptr = NULL;
  }

  // Disable interrupts until enabled in ControllerEnable().
  CT_INTC.GER_bit.EN_HINT_ANY = 0;

  // Clear pending system event enabled.  All system events are disabled.
  // TODO: this will clobber another PRU.  should only handle events
  // this PRU will use.
  CT_INTC.SECR0 = 0xffffffff;
  CT_INTC.SECR1 = 0xffffffff;
}

void InterruptHandlerInit(uint8_t evt, InterruptHandler *handler, Args args) {
  __controller.handler[evt] = handler;
  __controller.args[evt] = args;
}

void ServiceInterrupts(void) {
  while ((__R31 & ARM_TO_PRU_IRQ) != 0) {
    uint8_t evt = HIPRIO_EVT;

    // Note: Can't use debug.h helpers here because they run longer
    // than one IEP cycle and lead to an endless interrupt cycle

    // Unblock all and prioritize to run immediately.
    if (__controller.handler[evt] != NULL) {
      (__controller.handler[evt])(__controller.args[evt]);
    }

    CT_INTC.SICR_bit.STS_CLR_IDX = evt;
  }
}

// ControllerEnable starts the interrupt controller.
void ControllerEnable(void) {
  CT_INTC.GER_bit.EN_HINT_ANY = 1;
}
