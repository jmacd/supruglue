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

// Test-only hook invoked after every GPIO_SetRegister write, so a simulated
// peripheral (e.g. a UI-1203 meter) can react to output changes. The hook is
// not re-entered for writes it performs itself.
typedef void (*GPIO_WriteHook)(void *ctx, gpio_bank *bank, int reg, uint32_t value);

void GPIO_TestSetWriteHook(GPIO_WriteHook hook, void *ctx);

// Test-only: reset all banks and clear any installed hook.
void GPIO_TestReset(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_TEST32_GPIO_H
