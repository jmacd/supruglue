// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_RPMSG_TEST32_H
#define LIB_RPMSG_RPMSG_TEST32_H

#include "lib/rpmsg/rpmsg-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/rpmsg/am335x/rpmsg.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/rpmsg/test32/rpmsg.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/rpmsg/rpmsg-defs.h"

#define PRU_RPMSG_NO_BUF_AVAILABLE -7

typedef struct _TestTransport TestTransport;

TestTransport *NewTestTransport(void);

int  HostRecv(TestTransport *transport, void *data, uint16_t *len);
int  HostSend(TestTransport *transport, const void *data, uint16_t len);
void HostTransientReceiveError(TestTransport *transport);
void HostTransientSendError(TestTransport *transport);

#ifdef __cplusplus
}
#endif

#endif // LIB_RPMSG_RPMSG_TEST32_H
