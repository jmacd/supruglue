// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "supruglue/pinmap.h"
#include "supruglue/soc.h"
#include <stdint.h>

#ifndef SUPRUGLUE_GPIO_H
#define SUPRUGLUE_GPIO_H

// See TRM 25.4.1.x
//
// Note these constants are defined in the am335x TRM, but are used
// generally here because they are just integers.  Let's hope they're
// the same in other platforms.

enum _GpioBankNumber {
  GPIOREG_REVISION = NUM_WORDS(0x0),         // Section 25.4.1.1
  GPIOREG_SYSCONFIG = NUM_WORDS(0x10),       // Section 25.4.1.2
  GPIOREG_EOI = NUM_WORDS(0x20),             // Section 25.4.1.3
  GPIOREG_IRQSTATUS_RAW_0 = NUM_WORDS(0x24), // Section 25.4.1.4
  GPIOREG_IRQSTATUS_RAW_1 = NUM_WORDS(0x28), // Section 25.4.1.5
  GPIOREG_IRQSTATUS_0 = NUM_WORDS(0x2C),     // Section 25.4.1.6
  GPIOREG_IRQSTATUS_1 = NUM_WORDS(0x30),     // Section 25.4.1.7
  GPIOREG_IRQSTATUS_SET_0 = NUM_WORDS(0x34), // Section 25.4.1.8
  GPIOREG_IRQSTATUS_SET_1 = NUM_WORDS(0x38), // Section 25.4.1.9
  GPIOREG_IRQSTATUS_CLR_0 = NUM_WORDS(0x3C), // Section 25.4.1.10
  GPIOREG_IRQSTATUS_CLR_1 = NUM_WORDS(0x40), // Section 25.4.1.11
  GPIOREG_IRQWAKEN_0 = NUM_WORDS(0x44),      // Section 25.4.1.12
  GPIOREG_IRQWAKEN_1 = NUM_WORDS(0x48),      // Section 25.4.1.13
  GPIOREG_SYSSTATUS = NUM_WORDS(0x114),      // Section 25.4.1.14
  GPIOREG_CTRL = NUM_WORDS(0x130),           // Section 25.4.1.15
  GPIOREG_OE = NUM_WORDS(0x134),             // Section 25.4.1.16
  GPIOREG_DATAIN = NUM_WORDS(0x138),         // Section 25.4.1.17
  GPIOREG_DATAOUT = NUM_WORDS(0x13C),        // Section 25.4.1.18
  GPIOREG_LEVELDETECT0 = NUM_WORDS(0x140),   // Section 25.4.1.19
  GPIOREG_LEVELDETECT1 = NUM_WORDS(0x144),   // Section 25.4.1.20
  GPIOREG_RISINGDETECT = NUM_WORDS(0x148),   // Section 25.4.1.21
  GPIOREG_FALLINGDETECT = NUM_WORDS(0x14C),  // Section 25.4.1.22
  GPIOREG_DEBOUNCENABLE = NUM_WORDS(0x150),  // Section 25.4.1.23
  GPIOREG_DEBOUNCINGTIME = NUM_WORDS(0x154), // Section 25.4.1.24
  GPIOREG_CLEARDATAOUT = NUM_WORDS(0x190),   // Section 25.4.1.25
  GPIOREG_SETDATAOUT = NUM_WORDS(0x194),     // Section 25.4.1.26
};

typedef enum _GpioBankNumber GpioBankNumber;

typedef uint32_t gpio_bank;

void GPIO_SetRegister(gpio_bank *g, int r, uint32_t v);

uint32_t GPIO_GetRegister(gpio_bank *g, int r);

typedef struct {
  gpio_bank *bank;
  int        bit;
} gpio_pin;

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

#endif // SUPRUGLUE_GPIO_H
