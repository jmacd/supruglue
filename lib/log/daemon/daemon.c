// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    Entry entry;

    JournalRead(&__system.journal, &entry, JR_BLOCKING);

    int err;
    while ((err = ClientSend(&__transport, &entry, sizeof(entry))) != 0) {
      if (err == PRU_RPMSG_NO_PEER_ADDR) {
        // TODO block on init process, instead, because it knows the
        // destination first?  the host0 interrupt could be a kick, or
        // it could be something else.
        BlockOnHost0(&__controller);
        continue;
      }

      if (err == PRU_RPMSG_NO_BUF_AVAILABLE) {
        BlockOnHost0(&__controller);
        continue;
      }

      // TODO: otherwise, not clear what kind of fallback reporting
      // can be done when a permanent error is returned.
      // flash(4);
      Yield();
    }

    Yield();
  }
}
