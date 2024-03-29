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
    SemaDown(&__clock_lock);

    Timestamp clk;
    ReadClock(&clk);

    ThreadList *p = __asleep.next;
    while (p != &__asleep) {
      Thread *th = ThreadListEntry(p);

      p = p->next;
      int runnable = clk.CYCLES >= th->when.CYCLES;
      if (runnable) {
        ThreadListRemove(th);
        ThreadListPushFront(&__system_runnable, th);
      }
    }
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
