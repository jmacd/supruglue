// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/time.h"
#include "lib/coroutine/coroutine.h"
#include "lib/thread/thread.h"

SUPRUGLUE_DEFINE_THREAD(clock, 256);
SUPRUGLUE_DEFINE_SCHEDULE(schedule, 8);

// this is a min heap

void heapUp(int32_t position) {
  while (1) {
    int32_t parent = (position - 1) / 2;

    if (parent == position || schedule.pending.queue[parent].when < schedule.pending.queue[position].when) {
      break;
    }
    ScheduleItem tmp = schedule.pending.queue[position];
    schedule.pending.queue[position] = schedule.pending.queue[parent];
    schedule.pending.queue[parent] = tmp;
    position = parent;
  }
}

int Sleep(duration_t d) {
  if (schedule.pending.size == sizeof(schedule.pending.queue)) {
    return -1;
  }

  // see container/heap.Push
  ScheduleItem *item = &schedule.pending.queue[schedule.pending.size++];
  item->when = 0; //@@@
  item->lptr = &__system_current->list;

  heapUp(schedule.pending.size - 1);

  YieldBlocked();

  return 0;
}

void clockProcess(ThreadID thid, Args args) {
  // TODO: job is to ensure the cycle counter is reset before it resets
  // 1. find runnables with expired clocks
  // 2. re-heapify
  // one runnable at a time, ensure clock reschedules fast?
  while (1) {
  }
}

int TimeInit(void) {
  // Enable a handler to maintain the clock.
  Args args;
  args.ptr = "";
  return Create(&clock.thread, clockProcess, args, "clock", sizeof(clock.space));
}
