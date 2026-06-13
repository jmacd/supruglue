// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <algorithm>

#include "gpio.h"

static uint32_t       __gpio_banks[4];
static GPIO_WriteHook __gpio_hook = nullptr;
static void          *__gpio_hook_ctx = nullptr;
static bool           __gpio_in_hook = false;

void GPIO_Init(void) {
}

void GPIO_TestSetWriteHook(GPIO_WriteHook hook, void *ctx) {
  __gpio_hook = hook;
  __gpio_hook_ctx = ctx;
}

void GPIO_TestReset(void) {
  std::fill(std::begin(__gpio_banks), std::end(__gpio_banks), 0);
  __gpio_hook = nullptr;
  __gpio_hook_ctx = nullptr;
  __gpio_in_hook = false;
}

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

  // Notify a simulated peripheral, guarding against re-entry from writes the
  // hook itself performs.
  if (__gpio_hook != nullptr && !__gpio_in_hook) {
    __gpio_in_hook = true;
    __gpio_hook(__gpio_hook_ctx, as_int, r, v);
    __gpio_in_hook = false;
  }
}

uint32_t GPIO_GetRegister(gpio_bank *as_int, int r) {
  intptr_t   num = ((intptr_t)as_int) - 1;
  gpio_bank *g = &__gpio_banks[num];

  switch (r) {
  case GPIOREG_DATAOUT:
  case GPIOREG_DATAIN:
    return *g;

  default:
    throw std::invalid_argument("unknown gpio register");
  }
  return 0;
}
