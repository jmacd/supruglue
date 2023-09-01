// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/ui1203/ui1203.h"
#include <stdio.h>

void UI1203_Init_Reader(UI1203_Reader *rdr, gpio_pin clock, gpio_pin data) {
  rdr->clock = clock;
  rdr->data = data;
}

void UI1203_Run_Reader(UI1203_Reader *rdr) {
  // Placeholder! @@@
  printf("Running\n");
}
