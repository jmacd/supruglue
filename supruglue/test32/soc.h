// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
#define SUPRUGLUE_TEST_SUPRUGLUE_SOC_H

#include <stdint.h>

#include "supruglue/gpio.h"
#include "supruglue/soc.h"

#define WORDSZ sizeof(uint32_t)

typedef struct {
  gpio_register *gpio_banks;
} test_system;

test_system *Test_New_SOC();

void Test_Panic(const char *str);

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
