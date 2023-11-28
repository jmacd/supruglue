// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "soc.h"
#include "external/ti-pru-support/include/am335x/pru_cfg.h"

void *__dummy;

void SystemOnChipSetup(void) {
  // Allow OCP master port access by the PRU.
  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
}
