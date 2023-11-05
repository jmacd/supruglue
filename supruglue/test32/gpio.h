// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
#define SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H

#include <stdint.h>

#include "soc.h"

#define GPIOREG_CLEARDATAOUT 0
#define GPIOREG_SETDATAOUT 1
#define GPIOREG_DATAOUT 2
#define GPIOREG_DATAIN 3

#define GPIO_NUM_REGISTERS 4

#define GPIO_BANK0(soc) (&(soc)->gpio_banks[0])
#define GPIO_BANK1(soc) (&(soc)->gpio_banks[1])
#define GPIO_BANK2(soc) (&(soc)->gpio_banks[2])
#define GPIO_BANK3(soc) (&(soc)->gpio_banks[3])

#include "supruglue/gpio.h"

inline void GPIO_SetRegister(gpio_bank *g, int r, uint32_t v) {
  switch (r) {
  case GPIOREG_CLEARDATAOUT:
    *g &= ~v;
    break;

  case GPIOREG_SETDATAOUT:
    *g |= v;
    break;

  case GPIOREG_DATAIN:
    *g = v;
    break;

  default:
    throw std::invalid_argument("unknown gpio register");
  }
}

inline uint32_t GPIO_GetRegister(gpio_bank *g, int r) {
  switch (r) {
  case GPIOREG_DATAOUT:
    return *g;

  default:
    throw std::invalid_argument("unknown gpio register");
  }
  return 0;
}

#endif // SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
