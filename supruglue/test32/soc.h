// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
#define SUPRUGLUE_TEST_SUPRUGLUE_SOC_H

#include <algorithm>
#include <stdint.h>

#include "supruglue/gpio.h"
#include "supruglue/soc.h"

#define WORDSZ sizeof(uint32_t)

struct test_system {
  test_system();

  void Start(const std::function<void()> &f) { f(); }

  gpio_register *gpio_banks;
};

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
