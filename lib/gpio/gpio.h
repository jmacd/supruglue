// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_GPIO_GPIO_H
#define LIB_GPIO_GPIO_H

#include "lib/pinmap/pinmap.h"
#include "lib/soc/soc.h"
#include <stdint.h>

#include "lib/gpio/gpio-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/gpio/am335x/gpio.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/gpio/test32/gpio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
static inline int GPIO_GetPin(gpio_pin pin) {
  return (GPIO_GetRegister(pin.bank, GPIOREG_DATAIN) & (1 << pin.bit)) != 0;
}

static inline void GPIO_SetPin(gpio_pin pin, int value) {
  GPIO_SetRegister(pin.bank, value ? GPIOREG_SETDATAOUT : GPIOREG_CLEARDATAOUT, 1 << pin.bit);
}

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_H
