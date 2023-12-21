// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock.h"
#include "lib/thread/thread.h"

#include <stdio.h>

LockWord   __clock_lock;
ThreadList __asleep;

void Sleep(uint32_t cycles) {
  Thread *self = __system_current;

  ReadClock(&self->when);
  TimeAddCycles(&self->when, cycles);

  ThreadListPushBack(&__asleep, self);
  YieldBlocked();
}

void TimeAddCycles(Timestamp *clock, uint32_t cycles) {
  clock->CYCLES += cycles;
}
