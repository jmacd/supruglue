// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_DMA_DMA_H
#define LIB_DMA_DMA_H

#include "lib/dma/dma-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/dma/am335x/dma-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/dma/test32/dma-test32.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_H
