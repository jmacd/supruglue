// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

extern "C" {
#include "supruglue/test32/soc.h"
#include "stdio.h"
#include "supruglue/test32/gpio.h"
}

void Test_Panic(const char *str) {
  fprintf(stderr, "Panic! %s\n", str);
  uint32_t *ptr = 0;
  *ptr = 0;
}

test_system *Test_New_SOC() {
  test_system *ts = new test_system();
  ts->gpio_banks = new gpio_register[GPIO_NUM_REGISTERS]();
  return ts;
}
