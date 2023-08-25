// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "include/soc.h"
#include "stdio.h"

supruglue_test_system _SOC;

void Test_Panic(const char *str) {
  fprintf(stderr, "Panic! %s\n", str);
  uint32_t *ptr = 0;
  *ptr = 0;
}
