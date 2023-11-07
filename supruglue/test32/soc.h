// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
#define SUPRUGLUE_TEST_SUPRUGLUE_SOC_H

#include <stdint.h>

#define GPIO_NUM_REGISTERS 4

#include "supruglue/soc.h"

struct test_system {
  test_system();

  int32_t gpio_banks[GPIO_NUM_REGISTERS];
};

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
