// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
#define SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H

#include <stdint.h>

#include "soc.h"
#include "supruglue/gpio.h"

#define GPIOREG_CLEARDATAOUT 0
#define GPIOREG_SETDATAOUT 1
#define GPIOREG_DATAOUT 2
#define GPIOREG_DATAIN 3

#define GPIO_NUM_REGISTERS 4

#define GPIO_REGISTER0(soc) (&(soc)->gpio_banks[0])
#define GPIO_REGISTER1(soc) (&(soc)->gpio_banks[1])
#define GPIO_REGISTER2(soc) (&(soc)->gpio_banks[2])
#define GPIO_REGISTER3(soc) (&(soc)->gpio_banks[3])

#endif // SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
