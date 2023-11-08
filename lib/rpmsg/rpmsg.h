// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_RPMSG_H
#define LIB_RPMSG_RPMSG_H

#include "rpmsg-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/rpmsg/am335x/rpmsg.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/rpmsg/test32/rpmsg.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // LIB_RPMSG_RPMSG_H
