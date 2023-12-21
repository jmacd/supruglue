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

    if ((err = ClientRecv(&__transport, buf, &sz)) != 0) {
      Sleep(1000000);
    }

    Yield();
  }
}
