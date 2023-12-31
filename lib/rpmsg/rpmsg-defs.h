// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_RPMSG_DEFS_H
#define LIB_RPMSG_RPMSG_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ClientTransport ClientTransport;

extern ClientTransport __transport;

int ClientSend(ClientTransport *transport, const void *data, uint16_t len);
int ClientRecv(ClientTransport *transport, void *data, uint16_t *len);

#ifdef __cplusplus
}
#endif

#endif // LIB_RPMSG_RPMSG_DEFS_H
