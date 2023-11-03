// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_LOG_JOURNAL_JOURNAL_H
#define LIB_LOG_JOURNAL_JOURNAL_H

#include "lib/list/list.h"
#include "lib/sync/sync.h"
#include "lib/thread/thread.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const char *const overflowMessage;

typedef struct _Journal Journal;
typedef struct _Entry   Entry;
typedef struct _Block   Block;

#define NUM_PER_BLOCK 8
#define NUM_BLOCKS 4

SUPRUGLUE_DECLARE_LIST(BlockList, Block);

enum _JournalFlags {
  JF_NONE = 0,
  JF_BLOCKING = 1,
};

typedef enum _JournalFlags JournalFlags;

struct _Entry {
  // TODO: timestamp
  ThreadID    tid;
  const char *msg;
  int32_t     arg1;
  int32_t     arg2;
};

struct _Block {
  BlockList list;
  int32_t   count;
  int32_t   cursor;
  Entry     entries[NUM_PER_BLOCK];
};

struct _Journal {
  BlockList free;
  BlockList data;
  LockWord  lock;
  Block     blocks[NUM_BLOCKS];
};

void JournalInit(Journal *jl);
int  JournalRead(Journal *jl, Entry *record, JournalFlags flags);
void JournalWrite(Journal *jl, ThreadID tid, const char *msg, int32_t arg1, int32_t arg2);

#ifdef __cplusplus
}
#endif

#endif // LIB_LOG_JOURNAL_JOURNAL_H
