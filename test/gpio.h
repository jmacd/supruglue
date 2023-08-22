// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef TEST_GPIO_H
#define TEST_GPIO_H

#include "test/soc.h"

// See table 2.2 of the TRM for these addresses.
#define GPIO_BANK0 supruglue_test_soc.gpio1.registers
#define GPIO_BANK1 supruglue_test_soc.gpio2.registers
#define GPIO_BANK2 supruglue_test_soc.gpio3.registers
#define GPIO_BANK3 supruglue_test_soc.gpio4.registers

#endif // TEST_GPIO_H
