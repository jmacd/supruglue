// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <algorithm>

#include "gpio.h"

static uint32_t __gpio_banks[4];

void GPIO_SetRegister(gpio_bank *as_int, int r, uint32_t v) {
  intptr_t   num = ((intptr_t)as_int) - 1;
  gpio_bank *g = &__gpio_banks[num];

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

uint32_t GPIO_GetRegister(gpio_bank *as_int, int r) {
  intptr_t   num = ((intptr_t)as_int) - 1;
  gpio_bank *g = &__gpio_banks[num];

  switch (r) {
  case GPIOREG_DATAOUT:
    return *g;

  default:
    throw std::invalid_argument("unknown gpio register");
  }
  return 0;
}
