// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock.h"
#include "lib/thread/thread.h"

#include <stdio.h>

LockWord   __clock_lock;
ThreadList __asleep;

void SleepUntil(Timestamp *when, uint32_t cycles) {
  Thread *self = __system_current;

  TimeAddCycles(when, cycles);
  self->when = *when;

  ThreadListPushBack(&__asleep, self);
  YieldBlocked();

  // Note: the clock process could set the wakeup time as self->when
  // and the difference could be use to detect falling behind, maybe.
}
