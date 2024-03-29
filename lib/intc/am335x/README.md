
4.4.2.5 INTC Basic Programming Model
Follow these steps to configure the interrupt controller.
1. Set polarity and type of system event through the System Event Polarity Registers (SIPR1 and SPIR2)
and the System Event Type Registers (SITR1 and SITR2). Polarity of all system events is always high.
Type of all system events is always pulse.
2. Map system event to INTC channel through Channel Map registers.
3. Map channel to host interrupt through Host Interrupt Map registers. Recommend channel “x” be
mapped to host interrupt “x”.
4. Clear system event by writing 1s to SECR registers.
5. Enable host interrupt by writing index value to HIER register.
6. Enable interrupt nesting if desired.
7. Globally enable all interrupts through GER register.

ePWM
15.2.2.2 Proper Interrupt Initialization Procedure
When the ePWM peripheral clock is enabled it may be possible that interrupt flags may be set due to
spurious events due to the ePWM registers not being properly initialized. The proper procedure for
initializing the ePWM peripheral is:
1. Disable global interrupts (CPU INTM flag)
2. Disable ePWM interrupts
3. Initialize peripheral registers
4. Clear any spurious ePWM flags
5. Enable ePWM interrupts
6. Enable global interrupts

IEP
4.4.3.2.2 Basic Programming Model
Follow these basic steps to configure the IEP Timer.
1. Initialize timer to known state (default values)
a. Disable counter (GLB_CFG.CNT_ENABLE)
b. Reset Count Register (CNT) by writing 0xFFFFFFFF to clear
c. Clear overflow status register (GLB_STS.CNT_OVF)
d. Clear compare status (CMP_STS)
2. Set compare values (CMP0-CMPx)
3. Enable compare event (CMP_CFG.CMP_EN)
4. Set increment value (GLB_CFG.DEFAULT_INC)
5. Set compensation value (COMPEN.COMPEN_CNT)
6. Enable counter (GLB_CFG.CNT_ENABLE)

Cheetsheet on PRU INTC registers

0h REVID

Version information

4h CR

Control register: nesting mode

10h GER

The Global Host Interrupt Enable Register enables all the host
interrupts. Individual host interrupts are still enabled or disabled
from their individual enables and are not overridden by the global
enable.  1 bit.

1Ch GNLR

Global nesting level register

The Global Nesting Level Register allows the checking and setting of
the global nesting level across all host interrupts when automatic
global nesting mode is set. The nesting level is the channel (and all
of lower priority) that are nested out because of a current
interrupt. This register is only available when nesting is configured.

20h SISR Section 4.5.3.5

System Event Status Indexed Set Register

The System Event Status Indexed Set Register allows setting the status
of an event. The event to set is the index value written (0-63). This
sets the Raw Status Register bit of the given index.

24h SICR Section 4.5.3.6

System Event Status Indexed Clear Register

The System Event Status Indexed Clear Register allows clearing the
status of an event. The event to clear is the index value written
(0-63). This clears the Raw Status Register bit of the given index.

28h EISR Section 4.5.3.7

The System Event Enable Indexed Set Register allows enabling an
event. The event to enable is the index value written (0-63). This
sets the Enable Register bit of the given index.

2Ch EICR Section 4.5.3.8

The System Event Enable Indexed Clear Register allows disabling an
event. The event to disable is the index value written (0-63). This
clears the Enable Register bit of the given index.

34h HIEISR Section 4.5.3.9

The Host Interrupt Enable Indexed Set Register allows enabling a host
interrupt output. The host interrupt to enable is the index value
written (0-9). This enables the host interrupt output or triggers the
output again if already enabled.

38h HIDISR Section 4.5.3.10

The Host Interrupt Enable Indexed Clear Register allows disabling a
host interrupt output. The host interrupt to disable is the index
value written (0-9). This disables the host interrupt output.

80h GPIR Section 4.5.3.11

The Global Prioritized Index Register shows the event number of the
highest priority event pending across all the host interrupts.

200h SRSR0 Section 4.5.3.12

The System Event Status Raw/Set Register0 show the pending enabled
status of the system events 0 to 31. Software can write to the Status
Set Registers to set a system event without a hardware trigger. There
is one bit per system event

204h SRSR1 Section 4.5.3.13

Same: 32-63

280h SECR0 Section 4.5.3.14

The System Event Status Enabled/Clear Register0 show the pending
enabled status of the system events 0 to 31. Software can write to the
Status Clear Registers to clear a system event after it has been
serviced. If a system event status is not cleared then another host
interrupt may not be triggered or another host interrupt may be
triggered incorrectly. There is one bit per system event

284h SECR1 Section 4.5.3.15

Same, bits 32-63

300h ESR0 Section 4.5.3.16

The System Event Enable Set Register0 enables system events 0 to 31 to
trigger outputs. System events that are not enabled do not interrupt
the host. There is a bit per system event.

304h ESR1 Section 4.5.3.17

Same,

380h ECR0 Section 4.5.3.18

The System Event Enable Clear Register0 disables system events 0 to 31
to map to channels. System events that are not enabled do not
interrupt the host. There is a bit per system event.

384h ECR1 Section 4.5.3.19

Same,

400h CMR0 Section 4.5.3.20
404h CMR1 Section 4.5.3.21
408h CMR2 Section 4.5.3.22
40Ch CMR3 Section 4.5.3.23
410h CMR4 Section 4.5.3.24
414h CMR5 Section 4.5.3.25
418h CMR6 Section 4.5.3.26
41Ch CMR7 Section 4.5.3.27
420h CMR8 Section 4.5.3.28
424h CMR9 Section 4.5.3.29
428h CMR10 Section 4.5.3.30
42Ch CMR11 Section 4.5.3.31
430h CMR12 Section 4.5.3.32
434h CMR13 Section 4.5.3.33
438h CMR14 Section 4.5.3.34
43Ch CMR15 Section 4.5.3.35

The Channel Map Register0 specify the channel for the system events 0
to 3. There is one register per 4 system events. Note each CH_MAP_x
bitfield corresponds to a system event. Channel numbers (0-9) should
be written to these bitfields.

800h HMR0 Section 4.5.3.36

The Host Interrupt Map Register0 define the host interrupt for
channels 0 to 3. There is one register per 4 channels. Channels with
forced host interrupt mappings will have their fields read-only.

804h HMR1 Section 4.5.3.37
808h HMR2 Section 4.5.3.38

900h HIPIR0 Section 4.5.3.39

The Host Interrupt Prioritized Index Register0 shows the highest
priority current pending interrupt for the host interrupt 0. There is
one register per host interrupt.

904h HIPIR1 Section 4.5.3.40
908h HIPIR2 Section 4.5.3.41
90Ch HIPIR3 Section 4.5.3.42
910h HIPIR4 Section 4.5.3.43
914h HIPIR5 Section 4.5.3.44
918h HIPIR6 Section 4.5.3.45
91Ch HIPIR7 Section 4.5.3.46
920h HIPIR8 Section 4.5.3.47
924h HIPIR9 Section 4.5.3.48

D00h SIPR0 Section 4.5.3.49

The System Event Polarity Register0 define the polarity of the system
events 0 to 31. There is a polarity for each system event. The
polarity of all system events is active high; always write 1 to the
bits of this register.

D04h SIPR1 Section 4.5.3.50

D80h SITR0 Section 4.5.3.51

The System Event Type Register0 define the type of the system events 0
to 31. There is a type for each system event. The type of all system
events is pulse; always write 0 to the bits of this register.

D84h SITR1 Section 4.5.3.52


1100h HINLR0 Section 4.5.3.53

The Host Interrupt Nesting Level Register0 display and control the
nesting level for host interrupt 0. The nesting level controls which
channel and lower priority channels are nested. There is one register
per host interrupt.

1104h HINLR1 Section 4.5.3.54
1108h HINLR2 Section 4.5.3.55
110Ch HINLR3 Section 4.5.3.56
1110h HINLR4 Section 4.5.3.57
1114h HINLR5 Section 4.5.3.58
1118h HINLR6 Section 4.5.3.59
111Ch HINLR7 Section 4.5.3.60
1120h HINLR8 Section 4.5.3.61
1124h HINLR9 Section 4.5.3.62

1500h HIER Section 4.5.3.63

The Host Interrupt Enable Registers enable or disable individual host
interrupts. These work separately from the global enables. There is
one bit per host interrupt. These bits are updated when writing to the
Host Interrupt Enable Index Set and Host Interrupt Enable Index Clear
registers.

