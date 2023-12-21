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

void Sleep(uint32_t nanos);

void TimeAddCycles(Timestamp *clock, uint32_t cycles);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_H
