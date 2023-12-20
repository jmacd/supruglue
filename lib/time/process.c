// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "clock.h"
#include "lib/coroutine/coroutine.h"
#include "lib/intc/intc.h"
#include "lib/soc/sysevts.h"
#include "lib/sync/sync.h"
#include "process.h"

SUPRUGLUE_DEFINE_THREAD(clockproc, 256);

void clockProcess(ThreadID thid, Args args) {
  for (;;) {
    // solid(2);
    // flash(5);
    // solid(2);
    SemaDown(&__clock_lock);

    // solid(2);
    // flash(3); // + ThreadListLength(&__asleep));
    // solid(2);

    Timestamp clk;
    ReadClock(&clk);

    ThreadList *p = __asleep.next;
    while (p != &__asleep) {
      // solid(2);
      // flash(2);
      // solid(2);
      Thread *th = ThreadListEntry(p);

      int runnable = clk.CYCLES >= th->when.CYCLES;
      if (runnable) {
        ThreadListRemove(th);
      }
      p = p->next;
      if (runnable) {
        ThreadListPushFront(&__system_runnable, th);
      }
    }
    // solid(2);
    // flash(1);
    // solid(2);
  }
}

// ClockInit enables a handler to maintain the clock.
int ClockInit(void) {
  TimeInit();
  LockInit(&__clock_lock);
  ThreadListInit(&__asleep);

  Args args;
  args.ptr = "";
  return Create(&clockproc.thread, clockProcess, args, "clock", sizeof(clockproc.space));
}
