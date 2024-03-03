// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/gpio/gpio-defs.h"

#ifndef LIB_GPIO_AM335X_GPIO_H
#define LIB_GPIO_AM335X_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

// See table 2.2 of the TRM for these addresses.
#define GPIO_BANK0 ((gpio_bank *)0x44e07000)
#define GPIO_BANK1 ((gpio_bank *)0x4804c000)
#define GPIO_BANK2 ((gpio_bank *)0x481ac000)
#define GPIO_BANK3 ((gpio_bank *)0x481ae000)

inline void GPIO_SetRegister(gpio_bank *g, int r, uint32_t v) {
  g[r] = (gpio_bank)v;
}

inline uint32_t GPIO_GetRegister(gpio_bank *g, int r) {
  return (uint32_t)g[r];
}

inline void GPIO_Init(void) {
}

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_AM335X_GPIO_H
