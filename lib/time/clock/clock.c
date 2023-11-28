// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock/clock.h"
#include "lib/thread/thread.h"

#include <stdio.h>

// Note: I tested an implementation based on a heap, was about 500
// bytes larger.

ThreadList __asleep;

void Sleep(uint32_t d) {
  Thread *self = __system_current;

  ReadClock(&self->when);
  TimeAdd(&self->when, d);

  ThreadListPushBack(&__asleep, self);
  YieldBlocked();
}

void TimeAdd(Timestamp *clock, uint32_t dur) {
  clock->NANOS += dur;
}
