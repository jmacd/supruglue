// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_PWM_PWM_H
#define LIB_PWM_PWM_H

#include "lib/pinmap/pinmap.h"
#include "lib/soc/soc.h"
#include <stdint.h>

#include "lib/pwm/pwm-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/pwm/am335x/pwm-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/pwm/test32/pwm-test32.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // LIB_GPIO_GPIO_H
