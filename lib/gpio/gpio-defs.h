// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_GPIO_GPIO_DEFS_H
#define LIB_GPIO_GPIO_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t gpio_bank;

void GPIO_SetRegister(gpio_bank *g, int r, uint32_t v);

uint32_t GPIO_GetRegister(gpio_bank *g, int r);

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_DEFS_H
