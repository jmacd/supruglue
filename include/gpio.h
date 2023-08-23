// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_GPIO_H
#define SUPRUGLUE_GPIO_H

#include "include/soc.h"

// For clearing the GPIO registers
#define GPIOREG_CLEARDATAOUT (0x190 / WORDSZ)

// For setting the GPIO registers
#define GPIOREG_SETDATAOUT (0x194 / WORDSZ)

// For setting the GPIO registers
#define GPIOREG_DATAOUT (0x13C / WORDSZ)

// Number of registers in the GPIO bank
#define GPIOREG_MAX (0x198 / WORDSZ)

#if TESTING
#include "test/gpio.h"
#else
#include "include/am335x/gpio.h"
#endif

#endif // SUPRUGLUE_GPIO_H
