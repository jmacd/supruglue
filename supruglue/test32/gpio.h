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

#define GPIO_REGISTER_N(soc, n) (&(soc)->gpio_banks[n])

#define GPIO_REGISTER0(soc) GPIO_REGISTER_N(soc, 0)
#define GPIO_REGISTER1(soc) GPIO_REGISTER_N(soc, 1)
#define GPIO_REGISTER2(soc) GPIO_REGISTER_N(soc, 2)
#define GPIO_REGISTER3(soc) GPIO_REGISTER_N(soc, 3)

#endif // SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
