// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "soc.h"
#include "external/ti-pru-support/include/am335x/pru_cfg.h"
#include "external/ti-pru-support/include/am335x/pru_ctrl.h"

void *__dummy;

void SystemOnChipSetup(void) {
  // Allow OCP master port access by the PRU.
  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

#if SUPRUGLUE_PRU_NUM == 0
  PRU0_CTRL.WAKEUP_EN = PRU_HOST0_INTERRUPT;
#elif SUPRUGLUE_PRU_NUM == 1
  PRU1_CTRL.WAKEUP_EN = PRU_HOST1_INTERRUPT;
#endif
}

void Shutdown(void) {
  __system_shutdown = 1;
}
