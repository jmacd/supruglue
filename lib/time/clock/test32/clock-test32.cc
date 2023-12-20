// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/intc/intc.h"
#include "lib/time/clock/clock-defs.h"
#include <chrono>
#include <thread>
#include <unistd.h>

using std::thread;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

high_resolution_clock::time_point started;
thread                           *source;

void Tick(void) {
  SemaUp(&__clock_lock);
}

void TimeInit(void) {
  InterruptHandlerInit(1, Tick);
  source = new thread([] {
    for (;;) {
      usleep(10000);
      RaiseInterrupt(1);
    }
  });
}

void TimeStart(void) {
  started = high_resolution_clock::now();
}

void ReadClock(Timestamp *ts) {
  auto now = high_resolution_clock::now();

  ts->CYCLES = duration_cast<nanoseconds>(now - started).count() / 5;
}
