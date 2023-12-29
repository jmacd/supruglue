// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "absl/log/log.h"
#include "lib/intc/intc.h"
#include "lib/time/clock-defs.h"
#include <chrono>
#include <thread>
#include <unistd.h>

using std::thread;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

high_resolution_clock::time_point started;
high_resolution_clock::time_point switched;
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

void TimedSwitch(void) {
  auto now = high_resolution_clock::now();

  auto run = duration_cast<nanoseconds>(now - switched).count() / 5;

  __system_current->usage.run.CYCLES += run;
  __system_current->usage.stall.CYCLES += run;

  switched = now;
}
