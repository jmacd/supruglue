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
  // return (GPIO_GetRegister(pin.bank, GPIOREG_DATAIN) & (1 << pin.bit)) != 0;
  return (GPIO_GetRegister(pin.bank, GPIOREG_DATAIN) & (1 << pin.bit)) == 0 ? 0 : 1;
}

static inline void GPIO_SetPin(gpio_pin pin, int value) {
  GPIO_SetRegister(pin.bank, value ? GPIOREG_SETDATAOUT : GPIOREG_CLEARDATAOUT, 1 << pin.bit);
}

// static inline void GPIO_EnableOutput(gpio_pin pin) {
//   uint32_t current = GPIO_GetRegister(pin.bank, GPIOREG_OE);
//   uint32_t mask = ~(1 << pin.bit);
//   GPIO_SetRegister(pin.bank, GPIOREG_OE, current & mask);
// }

// static inline void GPIO_EnableInput(gpio_pin pin) {
//   uint32_t current = GPIO_GetRegister(pin.bank, GPIOREG_OE);
//   uint32_t mask = 1 << pin.bit;
//   GPIO_SetRegister(pin.bank, GPIOREG_OE, current & mask);
// }

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_H
