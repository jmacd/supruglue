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

// TODO generate something from irqgen's yaml input?  For now (for
// RPMsg to succeed), assume host interrupt 0 is all we need.

volatile register uint32_t __R31;

InterruptController __controller;

void ControllerInit(InterruptController *controller) {
  uint8_t evt;
  for (evt = 0; evt < 64; evt++) {
    ThreadListInit(&controller->waiting[evt]);
    // TODO Hmm maybe only need one thread? not one list per event?
  }

  // Disable interrupts until configured.
  CT_INTC.GER_bit.EN_HINT_ANY = 0;

  // Clear pending system event enabled.  All system events are disabled.
  CT_INTC.SECR0 = 0xffffffff;
  CT_INTC.SECR1 = 0xffffffff;

  // @@@ Note the following few lines can be replaced by setting the irq_map. TODO?

  // // Clear channel map register 1 (system events 4-7)
  // CT_INTC.CMR1 = 0x00000000;

  // // Set event 7 to channel 0
  // CT_INTC.CMR1_bit.CH_MAP_7 = 0;

  // // Map channel 1 to host 0
  // CT_INTC.HMR0_bit.HINT_MAP_1 = 0;

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

void ServiceInterrupts(InterruptController *controller) {
  if ((__R31 & PRU_HOST0_INTERRUPT) == 0) {
    return;
  }

  uint8_t evt = CT_INTC.HIPIR0;

  // TODO this has to move later after the condition is cleared
  if (evt >= 16 & evt < 32) {
    // software interrupts immediately cleared HACK @@@
    CT_INTC.SICR_bit.STS_CLR_IDX = evt;
  }

  // Unblock all and prioritize to run immediately.
  while (!ThreadListEmpty(&controller->waiting[evt])) {
    ThreadListPushBack(&__system_runnable, ThreadListPopFront(&controller->waiting[evt]));
  }
}

void BlockOnSystemEvent(InterruptController *controller, uint8_t evt) {
  ThreadListPushBack(&controller->waiting[evt], __system_current);
  YieldBlocked();
}

void ClearSystemEvent(uint8_t evt) {
  CT_INTC.SICR_bit.STS_CLR_IDX = evt;
}
