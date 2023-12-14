// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_iep.h"
#include "lib/time/clock/clock-defs.h"

Timestamp __clock;

void TimeInit(void) {
  __clock.NANOS_bit.HIGH = 0;
  __clock.NANOS_bit.LOW = 0;

  CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0;
  CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 5; // Count nanoseconds
  CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 0x1;   // W1ToClr
  CT_IEP.TMR_COMPEN_bit.COMPEN_CNT = 0;
  CT_IEP.TMR_CNT = 0xffffffff; // W1ToClr

  CT_IEP.TMR_CMP0 = 10000000;                   // 1/100 second
  CT_IEP.TMR_CMP_STS_bit.CMP_HIT = 0xFF;        // Write1toClear x 8
  CT_IEP.TMR_CMP_CFG_bit.CMP_EN = 0x1;          // CMP[0] enabled
  CT_IEP.TMR_CMP_CFG_bit.CMP0_RST_CNT_EN = 0x1; // CMP[0] will reset

  // @@@ Delay until after INTC is setup
  // CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 1;
}

// Device-Specific PRU Read Latency Values Appendix A.1 says 12 cycles
// to read IEP_TIMER.

void ReadClock(Timestamp *clock) {
  uint32_t ts = CT_IEP.TMR_CNT;
  if (ts < __clock.NANOS_bit.LOW) {
    __clock.NANOS_bit.HIGH++;
  }
  __clock.NANOS_bit.LOW = ts;
  *clock = __clock;
}
