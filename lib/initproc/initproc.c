// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "initproc.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"

// EH? Unclear purpose.  Is it important to call ClientRecv?  Or does
// knowing the ARM process is ready to talk matter?

void InitProcess(ThreadID thid, Args args) {
  for (;;) {
    int      err;
    char     buf[32];
    uint16_t sz = sizeof(buf);

    while ((err = ClientRecv(&__transport, buf, &sz)) != 0) {
      if (err == PRU_RPMSG_NO_BUF_AVAILABLE) {
        // flash(5);
        //  solid(1);
        //  BlockOnSystemEvent(&__controller, SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ);
        // Sleep(1000000);
      }

      // Sleep(1000000);
      //  flash(2);
      Yield();
    }
    Yield();
  }
}
