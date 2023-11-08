// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "debug.h"
#include "lib/gpio/gpio.h"
#include "lib/pinmap/pinmap.h"

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

void flash() {
  int i;
  for (i = 0; i < 5; i++) {
    uled1(0);
    uled2(0);
    uled3(0);
    uled4(0);
#if defined(SUPRUGLUE_AM335X)
    __delay_cycles(100000000);
#endif
    uled1(1);
    uled2(1);
    uled3(1);
    uled4(1);
#if defined(SUPRUGLUE_AM335X)
    __delay_cycles(100000000);
#endif
  }
}
