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

#define GPIO_BANK_N(soc, n) (&(soc)->gpio_banks[n])

#define GPIO_BANK0(soc) GPIO_BANK_N(soc, 0)
#define GPIO_BANK1(soc) GPIO_BANK_N(soc, 1)
#define GPIO_BANK2(soc) GPIO_BANK_N(soc, 2)
#define GPIO_BANK3(soc) GPIO_BANK_N(soc, 3)

#endif // SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
