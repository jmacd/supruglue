// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_iep.h"
#include "lib/time/clock/clock-defs.h"

// Device-Specific PRU Read Latency Values Appendix A.1 says 12 cycles
// to read IEP_TIMER.

// Note hard-coded can be variable?
#define RESOLUTION (1 << 16)

Timestamp __clock;

void TimeInit(void) {
  __clock = 0;

  CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0;
  CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 1; // Count cycles
  CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 0x1;   // W1ToClr
  CT_IEP.TMR_COMPEN_bit.COMPEN_CNT = 0;
  CT_IEP.TMR_CNT = 0xffffffff; // W1ToClr

  CT_IEP.TMR_CMP0 = RESOLUTION;                 // Power of two cycles
  CT_IEP.TMR_CMP_STS_bit.CMP_HIT = 0xFF;        // Write1toClear x 8
  CT_IEP.TMR_CMP_CFG_bit.CMP_EN = 0x1;          // CMP[0] enabled
  CT_IEP.TMR_CMP_CFG_bit.CMP0_RST_CNT_EN = 0x1; // CMP[0] will reset
}

void TimeStart(void) {
  // Called after INTC is setup.
  CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 1;
}

void ReadClock(Timestamp *clock) {
  uint32_t ts = CT_IEP.TMR_CNT;

  if (CT_IEP.TMR_CMP_STS_bit.CMP_HIT & 0x1) {
    CT_IEP.TMR_CMP_STS_bit.CMP_HIT = 0x1;
    __clock += RESOLUTION;
  }
  *clock = __clock | ts;
}
