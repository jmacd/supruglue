// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"
#include "lib/rpmsg/rpmsg_iface.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    Entry entry;

    if (JournalReadWait(&__system.journal, &entry, thid) < 0) {
      // TODO: e.g.,
      //   ThreadListPushBack(&ch->readers, __system.current);
      //   yieldInternal(JC_BLOCKED);
      continue;
    }

    while (ClientSend(&__transport, &entry, sizeof(entry)) != 0) {
      // TODO: not clear what kind of fallback reporting can be done
      // when a permanent error is returned.  Counters?
    }
  }
}
