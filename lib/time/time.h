// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_H
#define LIB_TIME_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/time/time-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/time/am335x/time.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/time/test32/time.h"
#endif

void Sleep(duration_t d);

int TimeInit(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_H
