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

struct soc_test32 {
  int32_t gpio_banks[GPIO_NUM_REGISTERS];
};

typedef struct soc_test32 soc_test32;

extern soc_test32 *__soc;

#ifdef __cplusplus
}
#endif

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
