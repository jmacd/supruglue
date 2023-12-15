// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_iep.h"

#include "lib/intc/intc.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock/clock-defs.h"

// Device-Specific PRU Read Latency Values Appendix A.1 says 12 cycles
// to read IEP_TIMER.

// This is power-of-two to simplify the logic.
#define RESOLUTION (1 << 16)

Timestamp __clock;

void UpdateClock(void) {
  uint32_t ts = CT_IEP.TMR_CNT;

  if (ts < __clock.CYCLES_bit.LOW) {
    __clock.CYCLES += RESOLUTION;
  }
  __clock.CYCLES_bit.LOW = ts;
}

void ClockHandler(void) {
  CT_IEP.TMR_CMP_STS_bit.CMP_HIT = 0x1;

  SemaUp(&__clock_lock);

  UpdateClock();
}

void TimeInit(void) {
  __clock.CYCLES = 0;

  CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0;
  CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 1; // Count cycles
  CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 0x1;   // W1ToClr
  CT_IEP.TMR_COMPEN_bit.COMPEN_CNT = 0;
  CT_IEP.TMR_CNT = 0xffffffff; // W1ToClr

  CT_IEP.TMR_CMP0 = RESOLUTION - 1;             // +1 resets next cycle
  CT_IEP.TMR_CMP_STS_bit.CMP_HIT = 0xFF;        // Write1toClear x 8
  CT_IEP.TMR_CMP_CFG_bit.CMP_EN = 0x1;          // CMP[0] enabled
  CT_IEP.TMR_CMP_CFG_bit.CMP0_RST_CNT_EN = 0x1; // CMP[0] will reset

  InterruptHandlerInit(SYSEVT_PR1_IEP_TIM_CAP_CMP_PEND, ClockHandler);
}

void TimeStart(void) {
  CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 1;
}

void ReadClock(Timestamp *clock) {
  UpdateClock();
  *clock = __clock;
}
