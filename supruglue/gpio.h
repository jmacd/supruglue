// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_GPIO_H
#define SUPRUGLUE_GPIO_H

#include <stdint.h>

typedef uint32_t gpio_register;

void GPIO_SetRegister(gpio_register *g, int r, uint32_t v);

uint32_t GPIO_GetRegister(gpio_register *g, int r);

typedef struct {
  gpio_register *reg;
  int bit;
} gpio_pin;

int GPIO_GetPin(gpio_pin pin);
void GPIO_SetPin(gpio_pin pin, int value);

#endif // SUPRUGLUE_GPIO_H
