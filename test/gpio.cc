// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

extern "C" {
#include "supruglue/test32/gpio.h"
}

void GPIO_SetRegister(supruglue_gpio *g, int r, uint32_t v) {
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
    Test_Panic("set register");
  }
}

uint32_t GPIO_GetRegister(supruglue_gpio *g, int r) {
  switch (r) {
  case GPIOREG_DATAOUT:
    return *g;

  default:
    Test_Panic("get register");
  }
  return 0;
}
