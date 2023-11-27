// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock/clock-defs.h"
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

void TimeInit(void) {
}

void ReadClock(Timestamp *ts) {
  auto now = high_resolution_clock::now();

  ts->NANOS = duration_cast<nanoseconds>(now.time_since_epoch()).count();
}
