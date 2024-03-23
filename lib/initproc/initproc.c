// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "initproc.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"

void initProcessThread(ThreadID thid, Args args) {
  for (;;) {
    // Try to receive until it succeeds.  This is necessary to get the
    // destination ID for the ARM host.
    char     buf[32];
    uint16_t sz = sizeof(buf);

    if (ClientRecv(&__transport, buf, &sz) != 0) {
      Sleep32(TIME_SECOND / 2);
      continue;
    }

    // Note we do not exit this loop. We needed to do the above once
    // per firmware start because there is only one host.
    for (;;) {
      Sleep64(__system.cfg.export_interval);

      Thread *th;
      for (th = __system.allthreads; th != NULL; th = th->allthreads) {
        METLOG_2u64(th, "cpu.run.cycles=%u cpu.stall.cycles=%u", th->usage.run.CYCLES, th->usage.stall.CYCLES);
      }
    }
  }
}

SUPRUGLUE_DEFINE_THREAD(init, 512);

int ProcessInit(void) {
  Args args;
  args.ptr = "";
  return Create(&init.thread, initProcessThread, args, "init", sizeof(init.space));
}
