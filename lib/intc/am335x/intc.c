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

  CT_INTC.SECR0 = 0xffffffff;
  CT_INTC.SECR1 = 0xffffffff;

  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ;
}

void ServiceInterrupts(InterruptController *controller) {
  if (__R31 & PRU_HOST0_INTERRUPT == 0) {
    return;
  }

  if (CT_INTC.SECR0 & (1 << SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ)) {
    // Specifically, we anticipate the RPMsg kick event.

    CT_INTC.SECR0 |= (1 << SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ);
  } else {

    if (CT_INTC.HIPIR0 & (1 << 31)) {
      // @@@???
    } else {
    }
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
