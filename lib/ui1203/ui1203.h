// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_UI1203_SUPRUGLUE_UI1203_H
#define LIB_UI1203_SUPRUGLUE_UI1203_H

#include "lib/gpio/gpio.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Maximum length of a decoded meter message (e.g. "R226107229550\r").
#define UI1203_MSG_MAX 32

// UI1203_Reader reads a Sensus encoder (UI-1203 protocol) meter using two
// GPIO lines:
//
//   clock_out: drives the meter's RED clock/power line. HIGH powers the meter;
//              each power cycle (LOW->HIGH) clocks out one bit. There is no
//              separate clock wire -- toggling power IS the clock.
//
//   data_in:   reads the meter's GREEN data line. The line is open-collector
//              (active LOW) and must be pulled up; firmware inverts it so a LOW
//              level reads as a 1 bit.
//
// The meter requires a continuous power-up period before it transmits, then
// emits ASCII bytes framed as: start(0) + 7 data bits (LSB first) + even
// parity + stop(1), terminated by a carriage return.
typedef struct {
  gpio_pin clock_out;
  gpio_pin data_in;

  char     message[UI1203_MSG_MAX];
  uint32_t length;
} UI1203_Reader;

// UI1203_Init_Reader configures the reader on the given clock and data pins and
// starts the reader thread.
void UI1203_Init_Reader(UI1203_Reader *rdr, gpio_pin clock, gpio_pin data);

// Internal: framing/parity codec shared with the unit tests.
//
// asciiToBits encodes a 7-bit ASCII character into a 10-bit UI-1203 frame.
// bitsToAscii decodes a 10-bit frame, returning the ASCII value or -1 when the
// start, parity, or stop bits are invalid.
int32_t asciiToBits(int32_t c);
int32_t bitsToAscii(int32_t b);

#ifdef __cplusplus
}
#endif

#endif // LIB_UI1203_SUPRUGLUE_UI1203_H
