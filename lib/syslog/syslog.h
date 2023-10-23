// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SYSLOG_PROC_H
#define LIB_SYSLOG_PROC_H

#include "lib/cpx/args.h"
#include "lib/cpx/cpx.h"

#ifdef __cplusplus
extern "C" {
#endif

// This is a cpx thread that reads log messages from other threads and
// conveys them over rpmsg to the host.

void SyslogProcess(ThreadID thid, Args args);

#ifdef __cplusplus
}
#endif

#endif // LIB_SYSLOG_PROC_H
