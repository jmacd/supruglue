// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_PINMAP_H
#define SUPRUGLUE_PINMAP_H

#include "supruglue.h"

#define GPIO_PIN_TO_BANK(pin) _JOIN(pin, _gpio_bank)
#define GPIO_PIN_TO_BIT(pin) _JOIN(pin, _gpio_bit)

#endif // SUPRUGLUE_PINMAP_H
