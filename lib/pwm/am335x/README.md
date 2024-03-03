
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
