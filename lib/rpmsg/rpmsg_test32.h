// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_RPMSG_TEST32_H
#define LIB_RPMSG_RPMSG_TEST32_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _RpmsgTransport RpmsgTransport;

RpmsgTransport *NewRpmsgTransport(void);

void StartTransport(RpmsgTransport *rpmt);
void StopTransport(RpmsgTransport *rpmt);

#ifdef __cplusplus
}
#endif

#endif // LIB_RPMSG_RPMSG_TEST32_H
