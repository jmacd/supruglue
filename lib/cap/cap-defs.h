// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CAP_CAP_DEFS_H
#define LIB_CAP_CAP_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/soc/soc.h"

void CAP_Init(void);
void CAP_Enable(void);
void CAP_ClearInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_CAP_CAP_DEFS_H
