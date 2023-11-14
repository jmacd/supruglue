// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_DEFS_H
#define LIB_TIME_TIME_DEFS_H

#include "lib/soc/soc.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t cycle_t;
typedef uint64_t duration_t;

typedef struct _Schedule     Schedule;
typedef struct _ScheduleItem ScheduleItem;

struct _ScheduleItem {
  cycle_t     when;
  ThreadList *lptr;
};

struct _Schedule {
  int32_t      size;
  ScheduleItem queue[0];
};

#define SUPRUGLUE_DEFINE_SCHEDULE(name, size) SUPRUGLUE_DEFINE_THINGS(name, Schedule, pending, ScheduleItem, size)

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_DEFS_H
