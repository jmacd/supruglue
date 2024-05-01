// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CAP_CAP_H
#define LIB_CAP_CAP_H

#include "lib/pinmap/pinmap.h"
#include "lib/soc/soc.h"
#include <stdint.h>

#include "lib/cap/cap-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/cap/am335x/cap-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/cap/test32/cap-test32.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_H
