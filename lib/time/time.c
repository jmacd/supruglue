// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/time.h"
#include "lib/coroutine/coroutine.h"
#include "lib/thread/thread.h"

SUPRUGLUE_DEFINE_THREAD(clock, 256);
SUPRUGLUE_DEFINE_SCHEDULE(schedule, 8);

void Sleep(duration_t d) {
  // TODO
  // 1. enter the heap
  // 2. active scheduler if not ...
  YieldBlocked();
}

void clockProcess(ThreadID thid, Args args) {
  // TODO: job is to ensure the cycle counter is reset before it resets
  // 1. find runnables with expired clocks
  // 2. re-heapify
  // one runnable at a time, ensure clock reschedules fast?
}

int TimeInit(void) {
  // Enable a handler to maintain the clock.
  Args args;
  args.ptr = "";
  return Create(&clock.thread, clockProcess, args, NewThreadConfig("clock", clock.stack, sizeof(clock.stack)));
}
