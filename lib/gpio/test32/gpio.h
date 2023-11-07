// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H
#define SUPRUGLUE_TEST_SUPRUGLUE_GPIO_H

#include <algorithm>
#include <stdint.h>

#include "lib/gpio/gpio-defs.h"
#include "lib/gpio/gpio.h"
#include "supruglue/test32/soc.h"

#define GPIO_BANK0(soc) (reinterpret_cast<gpio_bank *>(&(soc)->gpio_banks[0]))
#define GPIO_BANK1(soc) (reinterpret_cast<gpio_bank *>(&(soc)->gpio_banks[1]))
#define GPIO_BANK2(soc) (reinterpret_cast<gpio_bank *>(&(soc)->gpio_banks[2]))
#define GPIO_BANK3(soc) (reinterpret_cast<gpio_bank *>(&(soc)->gpio_banks[3]))

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
