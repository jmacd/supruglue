// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "pwm-am335x.h"
#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

void PWM_Init(void) {
  //////////////////////////////////////////////////////////////////////
  // Time-Base

  PWMSS0.EPWM_TBCTL = (3 << 14) | // FREE_SOFT: Free run
                      (0 << 13) | // PHSDIR: n/a in up-count mode
                      (0 << 10) | // CLKDIV: Time-base clock prescale bits
                      (1 << 7) |  // HSPCLKDIV: (Default) high-speed clock prescale bits
                      (0 << 6) |  // SWFSYNC: No force sync pulse
                      (0 << 4) |  // SYNCOSEL: Sync output from EPWMxSYNC
                      (0 << 3) |  // PRDLD: Load active period from shadow
                      (0 << 2) |  // PHSEN: Do not load from time-base phase register
                      (0 << 0);   // CTRMODE: Up-count mode

  // TBSTS: All defaults
  // TBPHSHR: Not used
  // TBPHS: Not used
  // TBCNT: Not used

  //////////////////////////////////////////////////////////////////////
  // Counter compare

  // CMPA: Not used
  // CMPAHR: Not used

  PWMSS0.EPWM_TBPRD = 1000; // TBPRD: Time-base period (16 bits)

  // CMPCTL: All defaults
  // CMPAHR: Not used
  PWMSS0.EPWM_CMPA = 500; // CMPB: Compare value for clearing EPWMxA
  PWMSS0.EPWM_CMPB = 750; // CMPB: Compare value for sample interrupt

  //////////////////////////////////////////////////////////////////////
  // Action qualifier

  PWMSS0.EPWM_AQCTLA = (2 << 0) | // ZRO: force EPWMxA high when TBCNT == 0
                       (1 << 4);  // CAU: force EPWMxA low when TBCNT == CMPA
  // AQCTLB: Not used
  // AQSFRC: Not used
  // AQCSFRC: Not used

  //////////////////////////////////////////////////////////////////////
  // Chopper: Not used
  // Deadband: Not used

  //////////////////////////////////////////////////////////////////////
  // Event trigger

  PWMSS0.EPWM_ETSEL = (1 << 3) | // INTEN: Enable interrupt
                      (6 << 0);  // INTSEL: CMPB incrementing
  PWMSS0.EPWM_ETPS = (1 << 0);   // INTPRD: Every event
}
