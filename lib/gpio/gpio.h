// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_GPIO_GPIO_H
#define LIB_GPIO_GPIO_H

#include "lib/pinmap/pinmap.h"
#include "supruglue/soc.h"
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

#define GPIO_PIN(soc, pin)                                                                                             \
  ({                                                                                                                   \
    gpio_pin __x;                                                                                                      \
    __x.bank = GPIO_PIN_TO_REGISTER(soc, pin);                                                                         \
    __x.bit = GPIO_PIN_TO_BIT_NUM(pin);                                                                                \
    __x;                                                                                                               \
  })

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_H
