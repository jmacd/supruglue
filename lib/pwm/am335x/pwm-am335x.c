// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "pwm-am335x.h"
#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

// Status: the code in this directory is a work-in-progress, and not
// completely functional.  Here, epwmss1 is initialized with a simple
// counting-up waveform, with:
// - Prescaler at 1/128
// - Period 10000
// - CMPA 5000
// - CMPB 7500
// - AQ: output low on TBCNT==0, high on TBCNT=CMPA (i.e., 50% duty)

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

// Various other things we know:
//
// 1. The paragraph from 15.2.2.9.3 indicates a connection with the
//    DMA controller, but it is unclear whether one needs to be
//    concerned with the DMA controller.  EDMA channel 35 == ePWMEVT1.
//    Do we need a dummy PaRaM entry so that the DMA controller is
//    satisfied and will allow interrupts to be generated?
//
// 2. Need to enable the TBCLK for EPWM1 using the PWMSS_CTRL
//    register (TRM 9.3.1.31) (offset = 664h)
//
// 3. Need to pinmux the EPWMxA output.
//
// 4. According to some forum threads, the device tree needs to be
//    configured to give the PRU control over it.
//    See https://e2e.ti.com/support/processors-group/processors/f/processors-forum/918237/am4378-arm-to-pru-event
//    See https://forum.beagleboard.org/t/pwmss-control-by-pru-with-kernel-4-19/31246/19
//    Using a device-tree with `status = "disabled"` to prevent the kernel from
//    adopting the PWM hardware.
//
//      ehrpwm1: pwm@200 {
//      	compatible = "ti,am3352-ehrpwm";
//      	#pwm-cells = <3>;
//      	reg = <0x200 0x80>;
//      	clocks = <&ehrpwm1_tbclk>, <&l4ls_gclk>;
//      	clock-names = "tbclk", "fck";
//      	status = "disabled";
//      };
//
// 5. Searched and searched for any examples or forum discussions on this
//    topic, found very little.  I assume that anyone trying to the eQEP
//    interrupts to work is in the same situation, so here's one:
//
//      https://e2e.ti.com/support/processors-group/processors/f/processors-forum/478720/beagle-bone-black-pru-not-able-to-initialize-pwmss0-or-pwmss1
//      https://e2e.ti.com/support/processors-group/processors/f/processors-forum/362435/am335x-unable-to-receive-pwm-interrupt-on-pru
//
// Current state: the PWM is configured and running correctly -- the PWM
// output can be observed by looping back to a GPIO pin, which the example
// `examples/pwm_outin` does.

// PWM_ClearInterrupt is called from the interrupt servicing routine.
//
void PWM_ClearInterrupt(void) {
  // Experimental: do we need to clear a DMA interrupt, too?
  // EDMA_BASE[SHADOW1(EDMAREG_ICRH)] = EDMA_dmaChannelMask;

  // Clear the event.
  PWMSS1.EPWM_ETCLR = 1;
}

// PWM_Init initializes but does not start the PWM.
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

  // Note: The pinmix step has been accomplished, for now, using
  // `config-pin P9_14 pwm`.  In theory, we can do this with the
  // control module:
  //
  // CONTROL_MODULE[0x848 / WORDSZ] = (6 << 0); // P9_14 == Mode 6

  // Enable the PWM clock.
  PWMSS1.CLKCONFIG_bit.EPWMCLK_EN = 1;

  // The following experimental code attempts to create a dummy PaRaM
  // entry, attempting to see if helps with the issue described in
  // 15.2.2.9.3.  It does not appear to help.

#if 0
  // Setup EDMA region access for Shadow Region 1
  // Note the Linux kernel uses Shadow Region 0.
  //
  // DRAE1 == DMA Region Access Enable shadow region 1.
  //
  // We enable a single channel in this region.
  EDMA_BASE[EDMA_DRAEH1] = EDMA_dmaChannelMask;
  EDMA_BASE[EDMA_DRAEH1] = EDMA_paramNumberMask;

  // // Map DMA Channel to PaRAM w/ same number.
  EDMA_BASE[EDMA_DCHMAP_N(EDMA_dmaChannel)] = EDMA_paramNumber << 5;

  // // Setup channel to submit to EDMA TC0 (highest priority).
  // //
  // // Note DMAQNUM_5 configures the channel controller for channels
  // // 32-39.  This is specific to channel 35.
  EDMA_BASE[EDMA_DMAQNUM_4] &= 0xFFFF8FFF;

  // // Enable channel for an event trigger.
  // EDMA_BASE[SHADOW1(EDMAREG_EESRH)] = EDMA_dmaChannelMask;

  EDMA_BASE[SHADOW1(EDMAREG_IESRH)] = EDMA_dmaChannelMask;

  // // Clear interrupt and secondary event registers.
  EDMA_BASE[SHADOW1(EDMAREG_SECRH)] = EDMA_dmaChannelMask;

  // // Clear event missed register.
  // EDMA_BASE[EDMA_EMCRH] = EDMA_dmaChannelMask;

  // // Setup and store Dummy PaRAM.
  uint16_t paramOffset = EDMA_PARAM_OFFSET;
  paramOffset += ((EDMA_paramNumber * EDMA_PARAM_SIZE) / WORDSZ);

  volatile edmaParam *edma_param_entry = (volatile edmaParam *)(EDMA_BASE + paramOffset);

  edma_param_entry->lnkrld.link = 0xFFFF;
  edma_param_entry->lnkrld.bcntrld = 0x0000;
  edma_param_entry->opt.static_set = 1;

  // Transfer complete interrupt enable.
  edma_param_entry->opt.tcinten = 1;

  // Intermediate transfer completion chaining enable.
  // not needed, used for splitting the transfer
  // edma_param_entry->opt.itcchen = 1;
  edma_param_entry->opt.tcc = EDMA_dmaChannel;

  edma_param_entry->ccnt.ccnt = 1;
  edma_param_entry->abcnt.acnt = 0;
  edma_param_entry->abcnt.bcnt = 1;
#endif
}

// PWM_Enable enables PWM interrupts.
void PWM_Enable(void) {
  PWMSS1.EPWM_ETSEL |= (1 << 3);
}
