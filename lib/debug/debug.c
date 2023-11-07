// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "debug.h"
#include "supruglue/gpio.h"
#include "supruglue/pinmap-arch.h"

void uled1(int val) {
  GPIO_SetPin(GPIO_PIN(__system, ULED1), val);
}
void uled2(int val) {
  GPIO_SetPin(GPIO_PIN(__system, ULED2), val);
}
void uled3(int val) {
  GPIO_SetPin(GPIO_PIN(__system, ULED3), val);
}
void uled4(int val) {
  GPIO_SetPin(GPIO_PIN(__system, ULED4), val);
}

void flash() {
  int i;
  for (i = 0; i < 5; i++) {
    uled1(0);
    uled2(0);
    uled3(0);
    uled4(0);
    __delay_cycles(100000000);
    uled1(1);
    uled2(1);
    uled3(1);
    uled4(1);
    __delay_cycles(100000000);
  }
}
