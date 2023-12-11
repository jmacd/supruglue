// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_PERIOD_PERIOD_H
#define LIB_TIME_PERIOD_PERIOD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/time/period/period-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/time/period/am335x/period-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/time/period/test32/period-test32.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_PERIOD_PERIOD_H
