// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "pwm-am335x.h"
#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

// 1. Disable global interrupts(CPUINTMflag)
// 2. Disablee PWM interrupts
// 3. Initialize peripheral registers
// 4. Clear any spuriouse PWM flags
// 5. Enable ePWM interrupts
// 6. Enable global interrupts

// Also need to enable the TBCLK for EPWM1
// 9.3.1.31 pwmss_ctrl Register (offset = 664h) [reset = 0h]

// Also need to pinmux the EPWMxA output

// See https://forum.beagleboard.org/t/pwmss-control-by-pru-with-kernel-4-19/31246/19
// Need a device tree hack.

#define CONTROL_MODULE ((uint32_t *)0x44E10000)

void PWM_ClearInterrupt(void) {
  PWMSS1.EPWM_ETCLR = (1 << 0);
}

void PWM_Init(void) {
  //////////////////////////////////////////////////////////////////////
  // Time-Base

  PWMSS1.EPWM_TBCTL = (3 << 14) | // FREE_SOFT: Free run
                      (0 << 13) | // PHSDIR: n/a in up-count mode
                      (7 << 10) | // CLKDIV: Time-base clock prescale bits (/128)
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

  PWMSS1.EPWM_TBPRD = 10000; // TBPRD: Time-base period (16 bits)

  // CMPCTL: All defaults
  // CMPAHR: Not used
  PWMSS1.EPWM_CMPA = 5000; // CMPB: Compare value for clearing EPWMxA
  PWMSS1.EPWM_CMPB = 7500; // CMPB: Compare value for sample interrupt

  //////////////////////////////////////////////////////////////////////
  // Action qualifier

  PWMSS1.EPWM_AQCTLA = (2 << 0) | // ZRO: force EPWMxA low when TBCNT == 0
                       (1 << 4);  // CAU: force EPWMxA high when TBCNT == CMPA
  // AQCTLB: Not used

  // @@@
  // PWMSS1.EPWM_AQSFRC = (3 << 6);  // Immediate mode
  // PWMSS1.EPWM_AQCSFRC = (1 << 0); // Force low on EPWMxA

  //////////////////////////////////////////////////////////////////////
  // Chopper: Not used
  // Deadband: Not used

  //////////////////////////////////////////////////////////////////////
  // Event trigger

  PWMSS1.EPWM_ETSEL = (1 << 3) | // INTEN: Enable interrupt
                      (6 << 0);  // INTSEL: CMPB incrementing
  PWMSS1.EPWM_ETPS = (1 << 0);   // INTPRD: Every event

  // Clear pending interrupt!
  PWMSS1.EPWM_ETCLR = (1 << 0);

  //////////////////////////////////////////////////////////////////////
  // Control module
  CONTROL_MODULE[0x664 / WORDSZ] = (1 << 1); // Enable TBCLK for EPWM1

  // Note: accomplis this with `config-pin P9_14 pwm`
  // CONTROL_MODULE[0x848 / WORDSZ] = (6 << 0); // P9_14 == Mode 6

  // PWMSS enablement?
  PWMSS1.CLKCONFIG_bit.EPWMCLK_EN = 1;
}
