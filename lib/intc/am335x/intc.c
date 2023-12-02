
// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/intc/am335x/intc.h"
#include "external/ti-pru-support/include/am335x/pru_intc.h"
#include "lib/debug/debug.h"
#include "lib/soc/sysevts.h"

// Interrupt inputs set bits 30 and 31 in register R31.
#define PRU_HOST0_INTERRUPT ((uint32_t)1 << 30)
#define PRU_HOST1_INTERRUPT ((uint32_t)1 << 31)

// TODO generate something from irqgen's yaml input?  For now (for
// RPMsg to succeed), assume host interrupt 0 is all we need.

volatile register uint32_t __R31;

InterruptController __controller;

void ControllerInit(InterruptController *controller) {
  ThreadListInit(&controller->blocked);

  CT_INTC.GER_bit.EN_HINT_ANY = 0;

  CT_INTC.SECR0 = 0xffffffff;
  CT_INTC.SECR1 = 0xffffffff;

  // Use EISR (indexed) or ESR (32bit) to enable system events
  // CT_INTC.EISR_bit.EN_SET_IDX = SYSEVT_PRU_TO_EDMA; // sysevt 18 / channel 9

  // CT_INTC.CMR4_bit.CH_MAP_18 = HOST_INTERRUPT_CHANNEL_PRU_TO_EDMA;
  // CT_INTC.HMR2_bit.HINT_MAP_9 = HOST_INTERRUPT_CHANNEL_PRU_TO_EDMA;
  // CT_INTC.HIEISR_bit.HINT_EN_SET_IDX = HOST_INTERRUPT_CHANNEL_PRU_TO_EDMA;

  // // Clear the system event mapped to the input interrupts.
  // CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_ARM_TO_PRU;
  // CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_EDMA_TO_PRU;
  // CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_EDMA_CTRL_ERROR_TO_PRU;
  // CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_EDMA_CHAN_ERROR_TO_PRU;

  // // Enable the EDMA (Transfer controller, Channel controller) clocks.
  // // TODO: Necessary? Probably the kernel does this.
  // CM_PER_BASE[CM_PER_TPTC0_CLKCTRL] = CM_PER_CLK_ENABLED;
  // CM_PER_BASE[CM_PER_TPCC_CLKCTRL] = CM_PER_CLK_ENABLED;

  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ;

  CT_INTC.GER_bit.EN_HINT_ANY = 1;
}

void ServiceInterrupts(InterruptController *controller) {
  if (__R31 & PRU_HOST0_INTERRUPT == 0) {
    return;
  }

  if (CT_INTC.SECR0 & (1 << SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ)) {
    // Specifically, we anticipate the RPMsg kick event.

    CT_INTC.SECR0 |= (1 << SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ);
  } else {

    // if (CT_INTC.HIPIR0 & (1 << 31)) {
    //   // @@@???
    // } else {
    // }
  }

  // Unblock all and prioritize to run immediately.
  while (!ThreadListEmpty(&controller->blocked)) {
    // Danger: don't push front? (Why?)
    ThreadListPushBack(&__system_runnable, ThreadListPopFront(&controller->blocked));
    // ThreadListPushFront(&__system_runnable, ThreadListPopFront(&controller->blocked));
  }
}

void BlockOnHost0(InterruptController *controller) {
  ThreadListPushBack(&controller->blocked, __system_current);
  YieldBlocked();
}
