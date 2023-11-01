// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_LOG_JOURNAL_JOURNAL_H
#define LIB_LOG_JOURNAL_JOURNAL_H

#include "lib/list/list.h"
#include "lib/thread/thread.h"
#include <stdint.h>

typedef struct _Journal Journal;
typedef struct _Entry   Entry;

#define NUM_JOURNAL_ENTRIES 16U

SUPRUGLUE_DECLARE_LIST(EntryList, Entry);

struct _Entry {
  // TODO: timestamp
  ThreadID    tid;
  const char *msg;
  int32_t     arg1;
  int32_t     arg2;
  EntryList   list;
};

struct _Journal {
  EntryList freelist;
  EntryList pending;
  Entry     space[NUM_JOURNAL_ENTRIES];
};

extern Journal __journal;

void JournalInit(Journal *jl);
int  JournalRead(Journal *jl, Entry *record);
void JournalWrite(Journal *jl, ThreadID tid, const char *msg, int32_t arg1, int32_t arg2);

#endif // LIB_LOG_JOURNAL_JOURNAL_H
