// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "initproc.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"

// EH? Unclear purpose.  Is it important to call ClientRecv?  Or does
// knowing the ARM process is ready to talk matter?

void InitProcess(ThreadID thid, Args args) {
  for (;;) {
    // Try to receive until it succeeds.  This is necessary to get the
    // destination ID for the ARM host.
    int      err;
    char     buf[32];
    uint16_t sz = sizeof(buf);

    if ((err = ClientRecv(&__transport, buf, &sz)) != 0) {
      Sleep(100000000);
      continue;
    }

    for (;;) {
      Sleep(2000000000);

      Thread *th;
      for (th = __system.allthreads; th != NULL; th = th->allthreads) {
        // @@@ important constant, make static and load via ELF
        PRULOG_2u64(INFO_BLOCK, "thread ran %u stalled %u", th->usage.run.CYCLES, th->usage.stall.CYCLES);
      }
    }
  }
}
