// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_GPIO_H
#define SUPRUGLUE_AM335X_GPIO_H

#include "include/soc.h"
#include "supruglue/gpio.h"

// See table 2.2 of the TRM for these addresses.
#define GPIO_BANK0 ((supruglue_gpio *)0x44e07000)
#define GPIO_BANK1 ((supruglue_gpio *)0x4804c000)
#define GPIO_BANK2 ((supruglue_gpio *)0x481ac000)
#define GPIO_BANK3 ((supruglue_gpio *)0x481ae000)

// See TRM 25.4.1.

// For clearing the GPIO registers
#define GPIOREG_CLEARDATAOUT NUM_WORDS(0x190)

// For setting the GPIO registers
#define GPIOREG_SETDATAOUT NUM_WORDS(0x194)

// For reading the GPIO registers
#define GPIOREG_DATAOUT NUM_WORDS(0x13C)

// For writing the GPIO registers
#define GPIOREG_DATAIN NUM_WORDS(0x138)

#endif // SUPRUGLUE_AM335X_GPIO_H
