// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_PINMAP_PINMAP_DEFS_H
#define LIB_PINMAP_PINMAP_DEFS_H

#include "lib/soc/soc.h"

#ifdef __cplusplus
extern "C" {
#endif

// These match the code generated in of tools/cmd/pinmap/pinmap.go

#define GPIO_PIN_TO_BANK_NUM(pin) _JOIN(pin, _GPIO_BANK_NUM)
#define GPIO_PIN_TO_BIT_NUM(pin) _JOIN(pin, _GPIO_BIT_NUM)
#define GPIO_PIN_TO_REGISTER(pin) pin##_GPIO_BANK

#ifdef __cplusplus
}
#endif

#endif // LIB_PINMAP_PINMAP_DEFS_H
