// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_UI1203_SUPRUGLUE_UI1203_H
#define LIB_UI1203_SUPRUGLUE_UI1203_H

#include "lib/gpio/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  gpio_pin data;
} UI1203_Reader;

typedef struct {
  gpio_pin data;
} UI1203_Writer;

// Note! Uses ePWM1 to generate clock.  Uses GPIO to read.
void UI1203_Init_Reader(UI1203_Reader *rdr, gpio_pin data);

// Note! Uses PRU-eCAP0 to receive clock.  Uses GPIO to write.
void UI1203_Init_Writer(UI1203_Writer *wr, gpio_pin data);

#ifdef __cplusplus
}
#endif

#endif // LIB_UI1203_SUPRUGLUE_UI1203_H
