// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INTC_INTC_H
#define LIB_INTC_INTC_H

#include "lib/intc/intc-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/intc/am335x/intc.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/intc/test32/intc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_INTC_H
