// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "pwm-am335x.h"
#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

void PWM_Init(void) {
  PWMSS0.EPWM_TBCTL = (3 << 14) | // FREE_SOFT: Free run
                      (0 << 13) | // PHSDIR: n/a in up-count mode
                      (0 << 10) | // CLKDIV: Time-base clock prescale bits
                      (1 << 7) |  // HSPCLKDIV: (Default) high-speed clock prescale bits
                      (0 << 6) |  // SWFSYNC: No force sync pulse
                      (2 << 4) |  // SYNCOSEL: Sync output select CMPB
                      (0 << 3) |  // PRDLD: Load active period from shadow
                      (0 << 2) |  // PHSEN: Do not load from time-base phase register
                      (0 << 0);   // CTRMODE: Up-count mode
  // TBSTS: All defaults
  // TBPHSHR: Not used
  // TBPHS: Not used
  // TBCNT: Not used
  // CMPA: Not used
  // CMPAHR: Not used

  PWMSS0.EPWM_TBPRD = 1000; // TBPRD: Time-base period (16 bits)

  // CMPCTL: All defaults
  // CMPA/CMPAHR: Not used
  PWMSS0.EPWM_CMPB = 500; // CMPB: Compare value for SYNCOSEL.

  PWMSS0.EPWM_AQCTLA = 0;
}
