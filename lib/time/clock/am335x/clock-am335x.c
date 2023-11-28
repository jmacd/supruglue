// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_iep.h"
#include "lib/time/clock/clock-defs.h"

Timestamp __clock;

void TimeInit(void) {
  __clock.NANOS_bit.HIGH = 0;
  __clock.NANOS_bit.LOW = 0;

  // The hardware reset state is for DEFAULT_INC is 5, which makes the
  // IEP counter on this device a nanosecond counter, i.e., (1000 / PRU_MHZ).
  // CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 5;
  CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 1;
}

// Device-Specific PRU Read Latency Values Appendix A.1 says 12 cycles
// to read EIP_TIMER.

void ReadClock(Timestamp *clock) {
  uint32_t ts = CT_IEP.TMR_CNT;
  if (ts < __clock.NANOS_bit.LOW) {
    __clock.NANOS_bit.HIGH++;
  }
  __clock.NANOS_bit.LOW = ts;
  *clock = __clock;
}
