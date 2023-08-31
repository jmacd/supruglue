// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_UI1203_SUPRUGLUE_UI1203_H
#define LIB_UI1203_SUPRUGLUE_UI1203_H

#include "supruglue/gpio.h"

typedef struct {
  gpio_pin clock;
  gpio_pin data;
} UI1203_Reader;

UI1203_Reader NewReader(gpio_pin clock, gpio_pin data);

#endif // LIB_UI1203_SUPRUGLUE_UI1203_H
