// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_H
#define LIB_TIME_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/time/clock-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/time/am335x/clock-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/time/test32/clock-test32.h"
#endif

void Sleep32(uint32_t cycles);
void TimeAddCycles32(Timestamp *clock, uint32_t cycles);
void SleepUntil32(Timestamp *when, uint32_t cycles);

inline void Sleep32(uint32_t cycles) {
  SleepUntil32(&__system_current->when, cycles);
}

inline void TimeAddCycles32(Timestamp *clock, uint32_t cycles) {
  clock->CYCLES += cycles;
}

void Sleep64(uint64_t cycles);
void TimeAddCycles64(Timestamp *clock, uint64_t cycles);
void SleepUntil64(Timestamp *when, uint64_t cycles);

inline void Sleep64(uint64_t cycles) {
  SleepUntil64(&__system_current->when, cycles);
}

inline void TimeAddCycles64(Timestamp *clock, uint64_t cycles) {
  clock->CYCLES += cycles;
}

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_H
