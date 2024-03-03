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
  }

  // Disable interrupts until configured.
  CT_INTC.GER_bit.EN_HINT_ANY = 0;

  // Clear pending system event enabled.  All system events are disabled.
  // TODO: this will clobber another PRU.  should only handle events
  // this PRU will use.
  CT_INTC.SECR0 = 0xffffffff;
  CT_INTC.SECR1 = 0xffffffff;

  // Unset the raw events
  CT_INTC.SICR_bit.STS_CLR_IDX = ARM_TO_PRU_EVT;
  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_PR1_IEP_TIM_CAP_CMP_PEND;
  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_EPWM1_INTR_PEND;
}

void InterruptHandlerInit(uint8_t evt, InterruptHandler *handler) {
  __controller.handler[evt] = handler;
}

void ServiceInterrupts(void) {
  while ((__R31 & ARM_TO_PRU_IRQ) != 0) {
    uint8_t evt = HIPRIO_EVT;

    // TODO: Can't understand why use of debug.h helpers
    // in this spot isn't working!

    // Unblock all and prioritize to run immediately.
    if (__controller.handler[evt] != NULL) {
      (__controller.handler[evt])();
    }

    CT_INTC.SICR_bit.STS_CLR_IDX = evt;
  }
}

void ControllerEnable(void) {
  // Re-enable events
  CT_INTC.GER_bit.EN_HINT_ANY = 1;
}
