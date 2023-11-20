// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/soc/test32/soc.h"

soc_test32 *__soc;

void SystemOnChipSetup() {
  __soc = (soc_test32 *)calloc(1, sizeof(soc_test32));
}

void SystemOnChipTeardown() {
  free(__soc);
  __soc = NULL;
}

void SystemOnChipDelay(int32_t cycles) {
  // TODO noop?
}
