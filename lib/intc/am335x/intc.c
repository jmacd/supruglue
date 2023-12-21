// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_intc.h"
#include "lib/debug/debug.h"
#include "lib/intc/am335x/intc.h"
#include "lib/soc/sysevts.h"

// Interrupt inputs set bits 30 and 31 in register R31.
#define PRU_HOST0_INTERRUPT 0x40000000
#define PRU_HOST1_INTERRUPT 0x80000000
#define PRU_HOST_ANY_INTERRUPT 0xc0000000

volatile register uint32_t __R31;

InterruptController __controller;

void ControllerInit(void) {
  uint8_t evt;
  for (evt = 0; evt < NUM_SYSEVTS; evt++) {
    __controller.handler[evt] = NULL;
  }

  // Disable interrupts until configured.
  CT_INTC.GER_bit.EN_HINT_ANY = 0;

  // Clear pending system event enabled.  All system events are disabled.
  CT_INTC.SECR0 = 0xffffffff;
  CT_INTC.SECR1 = 0xffffffff;

  // Use EISR (indexed) or ESR (32bit) to enable system events.
  //
  // Note: fewer instructions, maybe, if we assemble a bit map and
  // assign to ESR0/ESR1.
  CT_INTC.EISR_bit.EN_SET_IDX = SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ;
  CT_INTC.EISR_bit.EN_SET_IDX = SYSEVT_PR1_IEP_TIM_CAP_CMP_PEND;

  // Enable host interrupt 0
  // CT_INTC.HIER_bit.EN_HINT = 0x1;

  // Unset the raw events
  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ;
  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_PR1_IEP_TIM_CAP_CMP_PEND;

  // Re-enable events
  CT_INTC.GER_bit.EN_HINT_ANY = 1;
}

void InterruptHandlerInit(uint8_t evt, InterruptHandler *handler) {
  __controller.handler[evt] = handler;
}

void ServiceInterrupts(void) {
  while ((__R31 & PRU_HOST0_INTERRUPT) != 0) {
    uint8_t evt = CT_INTC.HIPIR0;

    // Unblock all and prioritize to run immediately.
    if (__controller.handler[evt] != NULL) {
      (__controller.handler[evt])();
    }

    CT_INTC.SICR_bit.STS_CLR_IDX = evt;
  }
}
