// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
#define SUPRUGLUE_TEST_SUPRUGLUE_SOC_H

#include "lib/soc/soc-defs.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void SystemOnChipShutdown(void);

#define ARCH_NUM_SYSEVTS 64

#ifdef __cplusplus
}
#endif

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
