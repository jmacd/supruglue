// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/syslog/syslog.h"
#include "lib/rpmsg/rpmsg_iface.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    LogEntry entry;

    // TODO: blocking read, no error.
    int ret = channelRead(&__system.log.ch, sizeof(__system.log.space), &entry, sizeof(entry));

    while ((ret = ClientSend(&__transport, &entry, sizeof(entry))) != 0) {
      // TODO: not clear what kind of fallback reporting can be done
      // when a permanent error is returned.  Counters?
    }
  }
}
