// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_H
#define LIB_TIME_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/time/clock/clock-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/time/clock/am335x/clock-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/time/clock/test32/clock-test32.h"
#endif

void Sleep(uint32_t nanos);

int ClockInit(void);

void TimeAdd(Timestamp *clock, uint32_t nanos);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_H
