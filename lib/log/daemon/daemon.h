// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SYSLOG_PROC_H
#define LIB_SYSLOG_PROC_H

#include "lib/args/args.h"
#include "lib/cpx/cpx.h"
#include "lib/rpmsg/rpmsg_iface.h"

#ifdef __cplusplus
extern "C" {
#endif

// Thread that reads log messages from other threads and conveys them
// over rpmsg to the host.

void SyslogProcess(ThreadID thid, Args args);

#ifdef __cplusplus
}
#endif

#endif // LIB_SYSLOG_PROC_H
