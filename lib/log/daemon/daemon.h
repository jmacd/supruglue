// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SYSLOG_PROC_H
#define LIB_SYSLOG_PROC_H

#include "lib/args/args.h"
#include "lib/log/journal/journal.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

// Thread function that reads log messages from other threads via the
// journal and conveys them over RPMsg to the host.

void SyslogProcess(ThreadID thid, Args args);

#ifdef __cplusplus
}
#endif

#endif // LIB_SYSLOG_PROC_H
