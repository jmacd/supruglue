// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/syslog/syslog.h"
#include "lib/rpmsg/rpmsg_iface.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    LogEntry entry;
    // TODO: Note no error return check, it's always 0.
    channelRead(&__system.log.ch, sizeof(__system.log.space), &entry, sizeof(entry));

    // TODO: Again, no error check.
    ClientSend(__transport, &entry, sizeof(entry));
  }
}
