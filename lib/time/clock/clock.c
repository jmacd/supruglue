// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock/clock.h"
#include "lib/thread/thread.h"

#include <stdio.h>

LockWord   __clock_lock;
ThreadList __asleep;

void Sleep(uint32_t d) {
  Thread *self = __system_current;

  ReadClock(&self->when);
  TimeAdd(&self->when, d);

  printf("SLEEPING\n");

  ThreadListPushBack(&__asleep, self);
  YieldBlocked();

  printf("WAKING\n");
}

void TimeAdd(Timestamp *clock, uint32_t dur) {
  clock->CYCLES += dur / 5;
}
