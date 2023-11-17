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

    flash(7);

    int err;
    while ((err = ClientSend(&__transport, &entry, sizeof(entry))) != 0) {
      if (err == PRU_RPMSG_NO_PEER_ADDR) {

        flash(4);
        // TODO block on init process?
        BlockOnHost0(&__controller);
        continue;
      }
      // flash(3);

      // TODO: otherwise, not clear what kind of fallback reporting
      // can be done when a permanent error is returned.
      Yield();
    }

    Yield();
  }
}
