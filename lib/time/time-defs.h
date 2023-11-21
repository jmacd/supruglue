// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_DEFS_H
#define LIB_TIME_TIME_DEFS_H

#include "lib/soc/soc.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Clock Clock;
typedef struct _Clock Duration;

typedef struct _Schedule     Schedule;
typedef struct _ScheduleItem ScheduleItem;

struct _Clock {
  union {
    volatile uint64_t NANOS;

    volatile struct {
      unsigned HIGH : 32;
      unsigned LOW : 32;
    } NANOS_bit;
  };
};

struct _ScheduleItem {
  Clock       when;
  ThreadList *lptr;
};

struct _Schedule {
  int32_t      size;
  ScheduleItem queue[0];
};

#define SUPRUGLUE_DEFINE_SCHEDULE(name, size) SUPRUGLUE_DEFINE_SIZED(name, Schedule, pending, ScheduleItem, size)

// Called by ClockInit().
void TimeInit(void);

void ReadClock(Clock *clock);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_DEFS_H
