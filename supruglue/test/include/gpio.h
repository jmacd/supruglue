// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
#define SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H

#include <stdint.h>

#include "include/soc.h"
#include "supruglue/gpio.h"

#define GPIOREG_CLEARDATAOUT 0
#define GPIOREG_SETDATAOUT 1
#define GPIOREG_DATAOUT 2
#define GPIOREG_DATAIN 3

#define GPIO_NUM_BANKS 4

#define GPIO_BANK0 (&_SOC_gpio0)
#define GPIO_BANK1 (&_SOC_gpio1)
#define GPIO_BANK2 (&_SOC_gpio2)
#define GPIO_BANK3 (&_SOC_gpio3)

extern supruglue_gpio _SOC_gpio0;
extern supruglue_gpio _SOC_gpio1;
extern supruglue_gpio _SOC_gpio2;
extern supruglue_gpio _SOC_gpio3;

#endif // SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
