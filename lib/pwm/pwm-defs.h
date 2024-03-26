// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_PWM_PWM_DEFS_H
#define LIB_PWM_PWM_DEFS_H

#include "lib/soc/soc.h"

#ifdef __cplusplus
extern "C" {
#endif

void PWM_Init(int32_t lowhigh, int32_t period, int32_t inter);
void PWM_ClearInterrupt(void);
void PWM_Enable(void);
void PWM_Disable(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_PWM_PWM_DEFS_H
