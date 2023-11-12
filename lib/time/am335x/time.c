// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "external/ti-pru-support/include/am335x/pru_ctrl.h"

#define PRU_CONTROL() (PRU_CORE_NUMBER() == 0 ? PRU0_CTRL : PRU1_CTRL)

uint32_t RawCycleCounter() {
  return PRU_CONTROL().CYCLE_bit.CYCLE;
}
