// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "supruglue/test32/soc.h"
#include "supruglue/test32/gpio.h"

test_system::test_system(int core_number) {
  this->core_number = core_number;
  this->gpio_banks = new gpio_register[GPIO_NUM_REGISTERS]();
}
