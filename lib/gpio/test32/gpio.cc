// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <algorithm>

#include "gpio.h"

void GPIO_SetRegister(gpio_bank *g, int r, uint32_t v) {
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

uint32_t GPIO_GetRegister(gpio_bank *g, int r) {
  switch (r) {
  case GPIOREG_DATAOUT:
    return *g;

  default:
    throw std::invalid_argument("unknown gpio register");
  }
  return 0;
}
