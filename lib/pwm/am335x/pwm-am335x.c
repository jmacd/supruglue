// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "pwm-am335x.h"
#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

// 1. Disable global interrupts(CPUINTMflag)
// 2. Disable PWM interrupts
// 3. Initialize peripheral registers
// 4. Clear any spurious PWM flags
// 5. Enable ePWM interrupts
// 6. Enable global interrupts

// Note that the interrupts coming from the ePWM module are also used
// as DMA events. The interrupt registers should be used to enable and
// clear the current DMA event in order for the ePWM module to
// generate subsequent DMA events.

// Note: EDMA channel 15 == ePWMEVT1

// Also need to enable the TBCLK for EPWM1
// 9.3.1.31 pwmss_ctrl Register (offset = 664h) [reset = 0h]

// Also need to pinmux the EPWMxA output

// See https://e2e.ti.com/support/processors-group/processors/f/processors-forum/918237/am4378-arm-to-pru-event

// See https://forum.beagleboard.org/t/pwmss-control-by-pru-with-kernel-4-19/31246/19
// Need a device tree hack.

// https://e2e.ti.com/support/processors-group/processors/f/processors-forum/478720/beagle-bone-black-pru-not-able-to-initialize-pwmss0-or-pwmss1

#define CONTROL_MODULE ((uint32_t *)0x44E10000)

// #define MPU_INTC_BASE ((uint32_t *)0x48200000)

void PWM_ClearInterrupt(void) {
  PWMSS1.EPWM_ETCLR = 1;
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

  // PWMSS1.EPWM_AQSFRC = (3 << 6);  // Immediate mode
  // PWMSS1.EPWM_AQCSFRC = (1 << 0); // Force low on EPWMxA

  //////////////////////////////////////////////////////////////////////
  // Chopper: Not used
  // Deadband: Not used

  //////////////////////////////////////////////////////////////////////
  // Event trigger

  PWMSS1.EPWM_ETSEL = (6 << 0); // INTSEL: CMPB incrementing
  PWMSS1.EPWM_ETPS = (1 << 0);  // INTPRD: Every event

  // Clear pending interrupt!
  PWMSS1.EPWM_ETCLR = (1 << 0);

  //////////////////////////////////////////////////////////////////////
  // Control module
  CONTROL_MODULE[0x664 / WORDSZ] = (1 << 1); // Enable TBCLK for EPWM1

  // Note: accomplis this with `config-pin P9_14 pwm`
  // CONTROL_MODULE[0x848 / WORDSZ] = (6 << 0); // P9_14 == Mode 6

  // PWMSS enablement?
  PWMSS1.CLKCONFIG_bit.EPWMCLK_EN = 1;

  // Setup EDMA region access for Shadow Region 1
  // Note the Linux kernel uses Shadow Region 0.
  //
  // DRAE1 == DMA Region Access Enable shadow region 1.
  //
  // We enable a single channel in this region.
  EDMA_BASE[EDMA_DRAEH1] = EDMA_dmaChannelMask;
  EDMA_BASE[EDMA_DRAEH1] = EDMA_paramNumberMask;

  // Map DMA Channel to PaRAM w/ same number.
  EDMA_BASE[EDMA_DCHMAP_N(EDMA_dmaChannel)] = EDMA_paramNumber << 5;

  // Setup channel to submit to EDMA TC0 (highest priority).
  //
  // Note DMAQNUM_5 configures the channel controller for channels
  // 32-39.  This is specific to channel 35.
  EDMA_BASE[EDMA_DMAQNUM_4] &= 0xFFFF8FFF;

  // Clear interrupt and secondary event registers.
  EDMA_BASE[SHADOW1(EDMAREG_SECRH)] = EDMA_dmaChannelMask;

  // Enable channel for an event trigger.
  EDMA_BASE[SHADOW1(EDMAREG_EESRH)] = EDMA_dmaChannelMask;

  // Clear event missed register.
  EDMA_BASE[EDMA_EMCRH] = EDMA_dmaChannelMask;

  // Setup and store Dummy PaRAM.
  uint16_t paramOffset = EDMA_PARAM_OFFSET;
  paramOffset += ((EDMA_paramNumber * EDMA_PARAM_SIZE) / WORDSZ);

  volatile edmaParam *edma_param_entry = (volatile edmaParam *)(EDMA_BASE + paramOffset);

  edma_param_entry->ccnt.ccnt = 1;
  edma_param_entry->abcnt.acnt = 0;
  edma_param_entry->abcnt.bcnt = 1;

  // MPU_INTC event 87 corresponds with ePWM1
  // 6.6.1.31 INTC_MIR_SET2 Register (offset = CCh) [reset = 0h]
  // MPU_INTC_BASE[0xCC / WORDSZ] = (1 << (87 - 64));
}

void PWM_Enable(void) {
  PWMSS1.EPWM_ETSEL |= (1 << 3);
}
