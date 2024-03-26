// OH MY GOODNESS
// TI Compiler uses "interrupt" as a keyword.  Can't use this as a C
// variable name and you will get really bad compiler messages if you do!

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

Cheatsheet on PWMSS

0h IDVER IP Revision Register Section 15.1.3.1

Version

4h SYSCONFIG System Configuration Register Section 15.1.3.2

Reset, standby, idle, emulation stuff.

8h CLKCONFIG Clock Configuration Register Section 15.1.3.3

Enable and stop the clock to the ePWM, eCAP, and eQEP.

Ch CLKSTATUS Clock Status Register Section 15.1.3.4

Shows the above config changes (which are async?)

Cheatsheet on PWM


0h TBCTL Time-Base Control Register Section 15.2.4.1

Clock divisor (rate 1/1 to 1/128); Phase direction (in up/down mode);
High-speed clock divisor; Software force sync; Sync output select;
Counter and Shadow modes

2h TBSTS Time-Base Status Register Section 15.2.4.2

Latch for synchronization events; overflow; counter direction.

4h TBPHSHR Extension for HRPWM Phase Register Section 15.2.4.3

High-res stuff.

6h TBPHS Time-Base Phase Register Section 15.2.4.4

High-res phase.

8h TBCNT Time-Base Counter Register Section 15.2.4.5

Time-based count (16 bits)

Ah TBPRD Time-Base Period Register Section 15.2.4.6

The PERIOD of the coutner.  When zero or max, ...

Eh CMPCTL Counter-Compare Control Register Section 15.2.4.7

Shadowing, and when to load/latch.

10h CMPAHR Extension for HRPWM Counter-Compare A Register Section 15.2.4.8

Compare-A high-res

12h CMPA Counter-Compare A Register Section 15.2.4.9

Value for compare-A reg

14h CMPB Counter-Compare B Register Section 15.2.4.10

Value for compare-A reg

16h AQCTLA Action-Qualifier Control Register for Output A (EPWMxA) Section 15.2.4.11
18h AQCTLB Action-Qualifier Control Register for Output B (EPWMxB) Section 15.2.4.12
1Ah AQSFRC Action-Qualifier Software Force Register Section 15.2.4.13
1Ch AQCSFRC Action-Qualifier Continuous S/W Force Register Set Section 15.2.4.14

1Eh DBCTL Dead-Band Generator Control Register Section 15.2.4.15
20h DBRED Dead-Band Generator Rising Edge Delay Count Register Section 15.2.4.16
22h DBFED Dead-Band Generator Falling Edge Delay Count Register Section 15.2.4.17

24h TZSEL Trip-Zone Select Register Section 15.2.4.18
28h TZCTL Trip-Zone Control Register Section 15.2.4.19
2Ah TZEINT Trip-Zone Enable Interrupt Register Section 15.2.4.20
2Ch TZFLG Trip-Zone Flag Register Section 15.2.4.21
2Eh TZCLR Trip-Zone Clear Register Section 15.2.4.22
30h TZFRC Trip-Zone Force Register Section 15.2.4.23

32h ETSEL Event-Trigger Selection Register Section 15.2.4.24
34h ETPS Event-Trigger Pre-Scale Register Section 15.2.4.25
36h ETFLG Event-Trigger Flag Register Section 15.2.4.26
38h ETCLR Event-Trigger Clear Register Section 15.2.4.27
3Ah ETFRC Event-Trigger Force Register Section 15.2.4.28

3Ch PCCTL PWM-Chopper Control Register Section 15.2.4.29
C0h HRCNFG HRPWM configuration register (HRCNFG) Section 15.2.4.30
