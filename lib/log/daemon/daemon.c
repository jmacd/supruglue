// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"
#include "lib/coroutine/coroutine.h"
#include "lib/rpmsg/rpmsg.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    Entry entry;

    JournalRead(&__system.journal, &entry, JR_BLOCKING);

    while (ClientSend(&__transport, &entry, sizeof(entry)) != 0) {
      // TODO: not clear what kind of fallback reporting can be done
      // when a permanent error is returned.  Counters?
    }

    Yield();
  }
}
