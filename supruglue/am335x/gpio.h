// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_GPIO_H
#define SUPRUGLUE_AM335X_GPIO_H

#include "soc.h"

#ifdef __cplusplus
extern "C" {
#endif

// See table 2.2 of the TRM for these addresses.
#define GPIO_BANK0(soc) ((gpio_bank *)0x44e07000)
#define GPIO_BANK1(soc) ((gpio_bank *)0x4804c000)
#define GPIO_BANK2(soc) ((gpio_bank *)0x481ac000)
#define GPIO_BANK3(soc) ((gpio_bank *)0x481ae000)

#include "supruglue/gpio.h"

inline void GPIO_SetRegister(gpio_bank *g, int r, uint32_t v) {
  g[r] = (gpio_bank)v;
}

inline uint32_t GPIO_GetRegister(gpio_bank *g, int r) {
  return (uint32_t)g[r];
}

#ifdef __cplusplus
}
#endif

#endif // SUPRUGLUE_AM335X_GPIO_H
