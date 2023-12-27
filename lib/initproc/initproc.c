// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "initproc.h"
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
      continue
    }

    // Send periodic metrics.
    // @@@ Need a list of all threads?
    // form rpmsg typeid 2
    // for {
    //   sleep...
    //   3 words per thread:
    //     TID
    //     RUN
    //     STALL
    // }
  }
}
