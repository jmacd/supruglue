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

enum _JournalReadFlags {
  JR_NONE = 0,
  JR_BLOCKING = 0x1,
};

enum _JournalWriteFlags {
  JW_NONE = 0,
  JW_YIELD = 0x1,
  JW_INFO = 0x10 | JW_YIELD,    // INFO will Yield
  JW_WARNING = 0x20 | JW_YIELD, // WARNING will Yield
  JW_FATAL = 0x40,              // FATAL will not Yield
};

typedef enum _JournalReadFlags  JournalReadFlags;
typedef enum _JournalWriteFlags JournalWriteFlags;

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
int  JournalRead(Journal *jl, Entry *record, JournalReadFlags flags);
void JournalWrite(Journal *jl, ThreadID tid, const char *fmt, int32_t arg1, int32_t arg2, JournalWriteFlags flags);

#define PRULOG_2U(level, fmt, arg1, arg2)                                                                              \
  JournalWrite(&__system.journal, TID(__system_current), (fmt), (arg1), (arg2), JW_##level)

#ifdef __cplusplus
}
#endif

#endif // LIB_LOG_JOURNAL_JOURNAL_H
