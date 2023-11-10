// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "debug.h"
#include "lib/gpio/gpio.h"
#include "lib/pinmap/pinmap.h"
#include "lib/soc/soc.h"

void uled1(int val) {
  GPIO_SetPin(GPIO_PIN(ULED1), val);
}
void uled2(int val) {
  GPIO_SetPin(GPIO_PIN(ULED2), val);
}
void uled3(int val) {
  GPIO_SetPin(GPIO_PIN(ULED3), val);
}
void uled4(int val) {
  GPIO_SetPin(GPIO_PIN(ULED4), val);
}

void blue(int val) {
  GPIO_SetPin(GPIO_PIN(P9_25), val);
}

void yellow(int val) {
  GPIO_SetPin(GPIO_PIN(P9_23), val);
}

void flash(int n) {
  int i;
  for (i = 0; i < n; i++) {
    uled1(0);
    uled2(0);
    uled3(0);
    uled4(0);
    yellow(0);
    blue(0);
    SystemOnChipDelay(100000000);
    uled1(1);
    uled2(1);
    uled3(1);
    uled4(1);
    yellow(1);
    blue(1);
    SystemOnChipDelay(100000000);
  }
}

void solid(int n) {
  int i;
  for (i = 0; i < n; i++) {
    uled1(1);
    uled2(1);
    uled3(1);
    uled4(1);
    SystemOnChipDelay(200000000);
  }
}
