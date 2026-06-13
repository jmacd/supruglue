This example reads a Sensus encoder (UI-1203 protocol) water meter, such as a
Sensus Omni R², using two GPIO lines on the PRU.

The UI-1203 protocol is synchronous and master-paced over three wires:

- **RED** — clock and power. There is no separate clock wire: toggling power is
  the clock. The meter must be held powered for ~3 seconds to reset, then each
  power cycle (off → on → sample) clocks out one bit. Driven here by
  `UI1203_CLOCK_PIN` (provisional `P9_25`).
- **GREEN** — data. Open-collector, active LOW, so it needs a pull-up; firmware
  inverts it (LOW = 1). Read here on `UI1203_DATA_PIN` (provisional `P9_23`).
- **BLACK** — ground.

Each byte is framed as start(0) + 7 data bits (LSB first) + even parity +
stop(1), and a reading is an ASCII string terminated by a carriage return
(e.g. `R226107229550\r`).

Electrical note: UI-1203 is nominally a 5 V protocol, but many meters read at
3.3 V. Because GREEN is open-collector, pulling it up to 3.3 V keeps the data
line safe for the (non-5 V-tolerant) BeagleBone regardless of the meter's
supply; only the RED clock/power line carries the supply voltage. Try 3.3 V
directly first; if the meter is unreliable, drive RED from a 5 V supply through
a GPIO-gated transistor/FET switch (data still pulled up to 3.3 V).

Status: the reader is implemented with plain GPIO bit-banging (no ePWM/eCAP/DMA,
so no device-tree changes for those peripherals are required). The framing
decoder, the RPMsg delivery of the decoded reading, and a full end-to-end host
simulation (a fake meter drives the data line in response to the reader's clock
toggles) are covered by unit tests in `lib/ui1203`. On-hardware bring-up and
pin/voltage validation are the remaining steps.
