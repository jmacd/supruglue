// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_UI1203_SUPRUGLUE_UI1203_H
#define LIB_UI1203_SUPRUGLUE_UI1203_H

#include "supruglue/gpio.h"

typedef struct {
  gpio_pin clock;
  gpio_pin data;
} UI1203_Reader;

void UI1203_Init_Reader(UI1203_Reader *rdr, gpio_pin clock, gpio_pin data);

void UI1203_Run_Reader(UI1203_Reader *rdr);

#endif // LIB_UI1203_SUPRUGLUE_UI1203_H
