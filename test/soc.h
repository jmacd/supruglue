// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef TEST_SOC_H
#define TEST_SOC_H

typedef struct {
  uint32_t registers[NUM_WORDS(GPIO_REG_)];
} supruglue_gpio;

typedef struct {
  supruglue_gpio gpio1;
  supruglue_gpio gpio2;
  supruglue_gpio gpio3;
  supruglue_gpio gpio4;
} supruglue_system;

extern SOC supruglue_test_soc;

#endif // TEST_SOC_H
