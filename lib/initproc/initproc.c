// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "initproc.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"

void InitProcess(ThreadID thid, Args args) {
  for (;;) {
    int      err;
    char     buf[32];
    uint16_t sz = sizeof(buf);

    while ((err = ClientRecv(&__transport, buf, &sz)) != 0) {
      if (err == PRU_RPMSG_NO_BUF_AVAILABLE) {
        // flash(5);
        // solid(1);
        BlockOnSystemEvent(&__controller, SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ);
        continue;
      }

      // TODO: otherwise, not clear what kind of fallback reporting
      // can be done when a permanent error (PRU_RPMSG_INVALID_HEAD)
      // is returned.  Try again!
      // flash(4);
      Yield();
    }
    Yield();
  }
}
