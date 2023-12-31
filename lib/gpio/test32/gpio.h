// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_GPIO_TEST32_GPIO_H
#define LIB_GPIO_TEST32_GPIO_H

#include <stdint.h>

#include "lib/gpio/gpio-defs.h"
#include "lib/soc/soc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_BANK0 ((gpio_bank *)1)
#define GPIO_BANK1 ((gpio_bank *)2)
#define GPIO_BANK2 ((gpio_bank *)3)
#define GPIO_BANK3 ((gpio_bank *)4)

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_TEST32_GPIO_H
