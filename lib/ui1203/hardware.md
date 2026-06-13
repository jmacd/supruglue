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

### Phase 1 — Schematic capture & review

- Capture in KiCad (open tooling, matches open-hardware intent).
- ~5 functional parts: 74AHCT125, data pull-up, 3-pin meter connector
  (terminal block or JST), BBB 2×23 headers, I²C EEPROM for cape ID.
- Add protection on the exposed meter lines: series resistors + TVS/clamp, plus
  decoupling caps. The meter cable leaves the enclosure, so ESD/transient
  protection matters for a field-deployed water-system device.

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
