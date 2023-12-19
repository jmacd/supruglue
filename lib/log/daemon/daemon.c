// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    Entry entry;

    JournalRead(&__system.journal, &entry, JR_BLOCKING);

    int err;
    while ((err = ClientSend(&__transport, &entry, sizeof(entry))) != 0) {
      if (err == PRU_RPMSG_NO_PEER_ADDR) {
        // BlockOnSystemEvent(&__controller, SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ);
        // flash(3);
        // Sleep(100000);
        // continue;
      }

      if (err == PRU_RPMSG_NO_BUF_AVAILABLE) {
        // BlockOnSystemEvent(&__controller, SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ);
        // flash(1);
        // Sleep(100000);
        // continue;
      }

      // TODO: otherwise, not clear what kind of fallback reporting
      // can be done when a permanent error is returned.
      // flash(4);
      Yield();
    }

    // flash(4);

    Yield();
  }
}
