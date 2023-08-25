// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_GPIO_H
#define SUPRUGLUE_GPIO_H

typedef uint32_t supruglue_gpio;

void GPIO_SetRegister(supruglue_gpio *g, int r, uint32_t v);

uint32_t GPIO_GetRegister(supruglue_gpio *g, int r);

#endif // SUPRUGLUE_GPIO_H
