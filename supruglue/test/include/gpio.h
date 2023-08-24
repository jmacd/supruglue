// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
#define SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H

#include <stdint.h>

#include "include/soc.h"
#include "supruglue/gpio.h"

#define GPIOREG_COUNT 3
#define GPIOREG_CLEARDATAOUT 0
#define GPIOREG_SETDATAOUT 1
#define GPIOREG_DATAOUT 2

typedef struct {
  uint32_t registers[NUM_WORDS(GPIOREG_COUNT)];
} supruglue_gpio;

#define GPIO_BANK0 _SOC_gpio1.registers
#define GPIO_BANK1 _SOC_gpio2.registers
#define GPIO_BANK2 _SOC_gpio3.registers
#define GPIO_BANK3 _SOC_gpio4.registers

extern supruglue_gpio _SOC_gpio1;
extern supruglue_gpio _SOC_gpio2;
extern supruglue_gpio _SOC_gpio3;
extern supruglue_gpio _SOC_gpio4;

#endif // SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
