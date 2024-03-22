// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_PWM_PWM_DEFS_H
#define LIB_PWM_PWM_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/soc/soc.h"

void PWM_Init(int32_t lowHigh, int32_t period, int32_t interrupt);
void PWM_ClearInterrupt(void);
void PWM_Enable(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_PWM_PWM_DEFS_H
