// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

extern "C" {
#include "include/soc.h"
#include "include/gpio.h"
#include "stdio.h"
}

void Test_Panic(const char *str) {
  fprintf(stderr, "Panic! %s\n", str);
  uint32_t *ptr = 0;
  *ptr = 0;
}

supruglue_test_system *Test_New_SOC() {
  supruglue_test_system *ts = new supruglue_test_system();
  ts->gpio_banks = new supruglue_gpio[GPIO_NUM_BANKS]();
  return ts;
}
