// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
#define SUPRUGLUE_TEST_SUPRUGLUE_SOC_H

#include <algorithm>
#include <stdint.h>

#include "supruglue/gpio.h"
#include "supruglue/soc.h"

#define WORDSZ sizeof(uint32_t)

#define PRU_CORE_NUMBER(soc) ((soc)->core_number)

struct test_system {
  test_system(int core_number);

  void Start(const std::function<void()> &f) {
    f();
  }

  int            core_number;
  gpio_register *gpio_banks;
};

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
