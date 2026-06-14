# UI-1203 / Sensus cape — hardware project notes

> **⚠️ WORK IN PROGRESS — HYPOTHETICAL RESEARCH.**
> Nothing here is committed-to or built. This document sketches the *outline*
> of a possible open-source hardware project to prototype, verify, and then
> produce a small run of a BeagleBone Black cape for reading UI-1203 / Sensus
> protocol water meters. Treat every cost, part number, and decision below as a
> placeholder to be confirmed after the breadboard hardware test succeeds.

## Goal

Open-source hardware for water systems: a small, reproducible BeagleBone Black
cape that interfaces a UI-1203 / Sensus protocol meter to the PRU, so the
reader in [`ui1203.h`](ui1203.h) can be deployed without breadboard wiring.

The circuit itself is tiny (see [the breadboard schematic](sensus_bbb_interface.svg)):
a 74AHCT125 buffer driving the 5 V clock/power line and a 3.3 V pull-up on the
open-collector data line. The "project" is mostly about turning that into a
documented, manufacturable, openly-licensed board.

## Phased outline

### Phase 0 — Breadboard verification (prerequisite)

Must succeed before any PCB work begins.

- Build the breadboard interface (74AHCT125 + pull-up) per the schematic.
- Read a real ZENNER Stealth Ultrasonic meter end-to-end with `UI1203_Reader`.
- **Lock the BBB pin assignment** (clock out, data in) avoiding HDMI/eMMC
  conflicts and confirming the chosen pins are PRU-drivable.
- Produce a working **device-tree overlay** for those pins. Everything
  downstream (EEPROM contents, silkscreen) depends on this being final.

#### Phase 0 shopping list

Already on hand: ZENNER Stealth Ultrasonic meter (ZSU/ZSUR) and the ZENNER
Stealth bare-3-wire interface cable.

**Core circuit (required)**

| Item | Spec / suggested part | Qty |
|------|-----------------------|-----|
| BeagleBone Black | Rev C (4 GB eMMC) — *if not already owned* | 1 |
| Level-up buffer | Adafruit 74AHCT125 breakout (#1787) | 1 |
| Data pull-up resistor | 10 kΩ, ¼ W (assortment pack is handy) | 1+ |
| Decoupling cap | 0.1 µF ceramic (74AHCT125 VCC) | 1–2 |
| Half-size breadboard | 400-tie | 1 |
| Jumper wires | M–M and M–F assortment | 1 pack |
| 3-pos screw terminal | breadboard-pitch, to land the bare meter cable | 1 |

**BBB host & imaging (required)**

| Item | Spec | Qty |
|------|------|-----|
| microSD card | 8–16 GB (flash the new Debian/PRU image) | 1 |
| microSD reader | if your machine lacks a slot | 1 |
| USB cable | mini-USB-B (BBB's port — not micro) for power + console | 1 |
| 5 V barrel supply | 5 V ≥ 2 A, 5.5×2.1 mm center-positive | 1 |
| Ethernet cable | for networking/SSH | 1 |

**Debug & test (strongly recommended)**

| Item | Why | Qty |
|------|-----|-----|
| USB logic analyzer | 8-ch, sigrok/PulseView-compatible — verify power-toggle bit-bang timing & 10-bit framing | 1 |
| 3.3 V USB-TTL serial cable (FTDI) | BBB J1 serial debug console for headless bring-up | 1 |
| Multimeter | confirm 5 V buffer output, continuity, pull-up | 1 |

**Optional line protection (nice for a field-facing prototype)**

| Item | Spec | Qty |
|------|------|-----|
| Series resistors | 100–330 Ω on the meter clock/data lines | 2 |
| TVS / clamp diode | low-capacitance, 3.3–5 V standoff, on exposed lines | 1–2 |

Minimum to get a first reading: BBB + 74AHCT125 breakout + 10 kΩ +
breadboard/jumpers + microSD + mini-USB + power. The logic analyzer is not
strictly required but saves hours when validating timing.

### Phase 1 — Schematic capture & review

- Capture in KiCad (open tooling, matches open-hardware intent).
- ~5 functional parts: 74AHCT125, data pull-up, 3-pin meter connector
  (terminal block or JST), BBB 2×23 headers, I²C EEPROM for cape ID.
- Add protection on the exposed meter lines: series resistors (~220 Ω, limit
  fault/inrush current) plus a **TVS** — a Transient Voltage Suppressor diode
  that clamps ESD/surge spikes to ground, protecting the not-5V-tolerant BBB
  GPIO. Plus per-IC decoupling caps (same 0.1 µF type as the Phase 0 BoM). These
  are already drawn on the prototype [schematic](sensus_bbb_interface.svg) and
  marked as recommended for robust field deployment.

### Phase 2 — Cape conformance & layout

- Conform to the **BeagleBone cape spec**: 3.4″ × 2.1″ outline, 2× 2×23
  headers, mounting holes.
- Include the **I²C EEPROM** (addr 0x54–0x57, jumper-selected) holding the cape
  ID string so the kernel auto-loads the overlay. This is the main non-trivial
  detail; start from a BeagleBoard reference cape design.
- Silkscreen: pin labels, meter wire colors (RED clk/pwr, GREEN data, BLACK
  gnd), polarity/voltage warnings.

### Phase 3 — Prototype fab (qty ~3–5)

- Fab + assembly via JLCPCB / PCBWay turnkey (all parts in standard libraries).
- Bring-up checklist: continuity, 5 V buffer output, EEPROM enumerates,
  overlay auto-loads, full meter read matches the breadboard result.
- Iterate the layout if needed.

### Phase 4 — Small production run (qty ~10–25)

- Re-spin with any prototype fixes; finalize BOM and assembly docs.
- Publish design as open-source hardware (see licensing below).
- Optional: enclosure / connector strain-relief for field use.

## Rough complexity & cost (placeholder)

| Item                     | Estimate                                  |
|--------------------------|-------------------------------------------|
| Design effort            | ~a weekend or two in KiCad for someone comfortable with it |
| Bare PCB                 | ~$2–5 / board                             |
| ~10 assembled boards     | ~$80–200 incl. assembly setup fees        |
| Turnaround               | ~1–2 weeks per fab cycle                  |

Biggest risks/unknowns: final pin choice + overlay, EEPROM programming flow,
and field-protection requirements — all resolved in Phases 0–2.

## Open-source hardware considerations

- License the hardware (e.g. **CERN-OHL** or **TAPR OHL**) and keep firmware/docs
  under the repository's existing license.
- Publish KiCad sources, Gerbers, BOM, and assembly notes alongside this repo so
  the board is reproducible — the point is open hardware for water systems, not
  a one-off.

## Status / next action

Blocked on **Phase 0**. No PCB work until the breadboard test passes and the pin
assignment + device-tree overlay are finalized.
