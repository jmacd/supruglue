// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <string.h>

#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

#include "pwm-am335x.h"

#define PWM_BASE PWMSS1

// TODO: generate DMA event defs
#define EDMA_dmaChannel 15
#define EDMA_dmaChannelMask (((uint32_t)1) << EDMA_dmaChannel)
#define EDMA_paramNumber EDMA_dmaChannel
#define EDMA_paramNumberMask (((uint32_t)1) << EDMA_paramNumber)

// Status:
// - Prescaler at 1/128
// - Period 10000
// - CMPA 5000
// - CMPB 7500
// - AQ: output low on TBCNT==0, high on TBCNT=CMPA (i.e., 50% duty)
// - Interrupt on CMPB

// PWM initialization procedure, from the am335x TRM 15.2.2.2:
//
// 1. Disable global interrupts(CPUINTMflag)
// 2. Disable PWM interrupts
// 3. Initialize peripheral registers
// 4. Clear any spurious PWM flags
// 5. Enable ePWM interrupts
// 6. Enable global interrupts

// Paragraph from the bottom of am335x TRM 15.2.2.9.3:
//
// Note that the interrupts coming from the ePWM module are also used
// as DMA events. The interrupt registers should be used to enable and
// clear the current DMA event in order for the ePWM module to
// generate subsequent DMA events.
//
// Current state: the PWM is configured and running correctly -- the PWM
// output can be observed by looping back to a GPIO pin, which the example
// `examples/pwm_outin` does.

// Note: The pinmix step has been accomplished, for now, using
// `config-pin P9_14 pwm`.  In theory, we can do this with the
// control module:
//
// CONTROL_MODULE[0x848 / WORDSZ] = (6 << 0); // P9_14 == Mode 6

// PWM_ClearInterrupt is called from the interrupt servicing routine.
//
void PWM_ClearInterrupt(void) {
  // Important! Clear the DMA interrupt, THEN clear the DMA trigger
  // event.
  EDMA_BASE[SHADOW1(EDMAREG_ICRH)] = 1U << 31;

  // Clear the DMA event
  EDMA_BASE[SHADOW1(EDMAREG_ECR)] = EDMA_dmaChannelMask;

  // Clear the flag.  Very important this has to happen after the above.
  PWM_BASE.EPWM_ETCLR = 1;
}

// PWM_Init initializes but does not start the PWM.
void PWM_Init(int32_t LH, int32_t P, int32_t I) {

  // Enable the PWM clock.

  PWM_BASE.CLKCONFIG_bit.EPWMCLK_EN = 1;
  PWM_BASE.SYSCONFIG_bit.IDLEMODE = 2;

  //////////////////////////////////////////////////////////////////////
  // Control module
  CONTROL_MODULE[0x664 / WORDSZ] = (1 << 1); // Enable TBCLK for EPWM1

  // DO NOT DO THIS: You can't setup the peripheral when the
  // (not sure) interface or functional clock is disabled.
  // //////////////////////////////////////////////////////////////////////
  // Control module
  // CONTROL_MODULE[0x664 / WORDSZ] = (0 << 1); // Enable TBCLK for EPWM1
  // Enable the PWM clock.
  // PWM_BASE.CLKCONFIG_bit.EPWMCLK_EN = 0;

  //////////////////////////////////////////////////////////////////////
  // Time-Base

  PWM_BASE.EPWM_TBCTL = (0 << 14) | // FREE_SOFT: Disabled.
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

  PWM_BASE.EPWM_TBCNT = 0; // TBCNT: Time-base clock (16 bits)
  PWM_BASE.EPWM_TBPRD = P; // TBPRD: Time-base period (16 bits)

  // CMPCTL: All defaults
  // CMPAHR: Not used

  PWM_BASE.EPWM_CMPA = LH; // CMPB: Compare value for clearing EPWMxA
  PWM_BASE.EPWM_CMPB = I;  // CMPB: Compare value for sample interrupt

  //////////////////////////////////////////////////////////////////////
  // Action qualifier

  PWM_BASE.EPWM_AQCTLA = (2 << 0) | // ZRO: force EPWMxA low when TBCNT == 0
                         (1 << 4);  // CAU: force EPWMxA high when TBCNT == CMPA
  // AQCTLB: Not used

  // PWM_BASE.EPWM_AQSFRC = (3 << 6);  // Immediate mode
  // PWM_BASE.EPWM_AQCSFRC = (1 << 0); // Force low on EPWMxA

  //////////////////////////////////////////////////////////////////////
  // Chopper: Not used
  // Deadband: Not used

  //////////////////////////////////////////////////////////////////////
  // Event trigger

  PWM_BASE.EPWM_ETCLR = (1 << 0); // Clear pending interrupt
  PWM_BASE.EPWM_ETSEL = (6 << 0); // INTSEL: CMPB incrementing
  PWM_BASE.EPWM_ETPS = (1 << 0);  // INTPRD: Every event

  // DMA setup
  // The following experimental code attempts to create a dummy PaRaM
  // entry, because there appears to be no way to "just send an event"
  // to one of the interrupt controllers.

  // Setup EDMA region access for Shadow Region 1
  // Note the Linux kernel uses Shadow Region 0.
  //
  // DRAE1 == DMA Region Access Enable shadow region 1.
  //
  // We enable a single channel in this region.
  EDMA_BASE[EDMA_DRAE1] = EDMA_dmaChannelMask;
  EDMA_BASE[EDMA_DRAEH1] = 1U << 31;

  // Map DMA Channel to PaRAM w/ same number.
  EDMA_BASE[EDMA_DCHMAP_N(EDMA_dmaChannel)] = EDMA_paramNumber << 5;

  // Note DMAQNUM_1 configures the channel controller for channels
  // 8-15.  This is specific to channel 15 => queue 2.
  EDMA_BASE[EDMA_DMAQNUM_1] = 0x20000000;

  // Enable channel for an event trigger.
  EDMA_BASE[SHADOW1(EDMAREG_EESR)] = EDMA_dmaChannelMask;
  EDMA_BASE[SHADOW1(EDMAREG_ICR)] = EDMA_dmaChannelMask;

  // Clear and enable the register (was added to shadow region above)
  EDMA_BASE[SHADOW1(EDMAREG_ICRH)] = 1U << 31;  // which is event 63
  EDMA_BASE[SHADOW1(EDMAREG_IESRH)] = 1U << 31; // which is event 63

  // Clear secondary & missed register
  EDMA_BASE[SHADOW1(EDMAREG_SECR)] = EDMA_dmaChannelMask;
  EDMA_BASE[EDMA_EMCR] = EDMA_dmaChannelMask;

  // Setup and store Dummy PaRAM.
  uint16_t paramOffset = EDMA_PARAM_OFFSET;
  paramOffset += ((EDMA_paramNumber * EDMA_PARAM_SIZE) / WORDSZ);

  volatile edmaParam *edma_param_entry = (volatile edmaParam *)(EDMA_BASE + paramOffset);
  memset((void *)edma_param_entry, 0, sizeof(*edma_param_entry));

  edma_param_entry->lnkrld.link = 0xFFFF;
  edma_param_entry->lnkrld.bcntrld = 0x0000;
  edma_param_entry->opt.static_set = 1;

  // Transfer complete interrupt enable.
  edma_param_entry->opt.tcinten = 1;
  edma_param_entry->opt.tcc = 63;

  edma_param_entry->ccnt.ccnt = 1;
  edma_param_entry->abcnt.acnt = 1;
  edma_param_entry->abcnt.bcnt = 1;
  edma_param_entry->src = 0x10000 + 1;
  edma_param_entry->dst = 0x10000;
}

// PWM_Enable enables PWM interrupts.
void PWM_Enable(void) {
  PWM_BASE.EPWM_TBCTL |= (3 << 14); // FREE_SOFT: Free run

  //////////////////////////////////////////////////////////////////////
  // Control module
  CONTROL_MODULE[0x664 / WORDSZ] = (1 << 1); // Enable TBCLK for EPWM1

  PWM_BASE.EPWM_ETSEL = (1 << 3) | // Interrupts enabled
                        (6 << 0);  // Interrupt on CMB-B == TBCNT
}

void PWM_Disable(void) {
  PWM_BASE.EPWM_TBCTL &= ~(3 << 14); // FREE_SOFT: Free run
}
