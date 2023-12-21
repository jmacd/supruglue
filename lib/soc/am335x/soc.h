// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_SOC_H
#define SUPRUGLUE_AM335X_SOC_H

#include "lib/soc/soc-defs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Defined in delay.s.
extern void SystemOnChipDelay(int cycles);

inline int SystemOnChipIsShutdown(void) {
  return 0;
}

#define ARCH_NUM_SYSEVTS 64

#ifdef __cplusplus
}
#endif

#endif // SUPRUGLUE_AM335X_SOC_H
