// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_RPMSG_TEST32_H
#define LIB_RPMSG_RPMSG_TEST32_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/rpmsg/am335x/rpmsg_am335x_iface.h"

typedef struct _TestTransport TestTransport;

TestTransport *NewTestTransport(void);

ClientTransport *GetClientTransport(TestTransport *host);

void StartTestTransport(TestTransport *host);
void StopTestTransport(TestTransport *host);

int HostSend(TestTransport *transport, const void *data, uint16_t len);
int HostRecv(TestTransport *transport, void *data, uint16_t *len);

#ifdef __cplusplus
}
#endif

#endif // LIB_RPMSG_RPMSG_TEST32_H
