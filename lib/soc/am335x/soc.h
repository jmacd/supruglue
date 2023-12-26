// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_SOC_H
#define SUPRUGLUE_AM335X_SOC_H

#include "lib/soc/soc-defs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Interrupt inputs set bits 30 and 31 in register R31.
#define PRU_HOST0_INTERRUPT 0x40000000
#define PRU_HOST1_INTERRUPT 0x80000000
#define PRU_HOST_ANY_INTERRUPT 0xc0000000

// Defined in delay.s.
extern void SystemOnChipDelay(int cycles);

// Defined in sleep.s.
extern void SystemOnChipSuspend(void);

inline int SystemOnChipIsShutdown(void) {
  return 0;
}

#define ARCH_NUM_SYSEVTS 64

#ifdef __cplusplus
}
#endif

#endif // SUPRUGLUE_AM335X_SOC_H
