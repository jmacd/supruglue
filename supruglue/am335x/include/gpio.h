// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_GPIO_H
#define SUPRUGLUE_AM335X_GPIO_H

#include "include/soc.h"

// See table 2.2 of the TRM for these addresses.
#define GPIO_BANK0 ((uint32_t *)0x44e07000)
#define GPIO_BANK1 ((uint32_t *)0x4804c000)
#define GPIO_BANK2 ((uint32_t *)0x481ac000)
#define GPIO_BANK3 ((uint32_t *)0x481ae000)

// For clearing the GPIO registers
#define GPIOREG_CLEARDATAOUT NUM_WORDS(0x190)

// For setting the GPIO registers
#define GPIOREG_SETDATAOUT NUM_WORDS(0x194)

// For setting the GPIO registers
#define GPIOREG_DATAOUT NUM_WORDS(0x13C)

#endif // SUPRUGLUE_AM335X_GPIO_H
