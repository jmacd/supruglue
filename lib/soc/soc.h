// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SOC_SOC_H
#define LIB_SOC_SOC_H

#include "lib/soc/soc-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/soc/am335x/soc.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/soc/test32/soc.h"
#endif

#endif // LIB_SOC_SOC_H
