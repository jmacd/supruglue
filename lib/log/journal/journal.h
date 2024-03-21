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

// The ThreadID that shows for the overflow message
#define OVERFLOW_THREAD_ID 0xd

extern const char *const overflowMessage;

typedef struct _Journal Journal;
typedef struct _Entry   Entry;
typedef struct _Block   Block;
typedef struct _Integer Integer;

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

  JW_METRICS = 0x10, // Metrics do not yield
  JW_INFO_NOYIELD = 0x20,
  JW_WARNING_NOYIELD = 0x40,
  JW_FATAL = 0x80, // Fatal does not yield

  JW_INFO = JW_INFO_NOYIELD | JW_YIELD,       // INFO will Yield
  JW_WARNING = JW_WARNING_NOYIELD | JW_YIELD, // WARNING will Yield

  JW_FMT_Au32 = 0x1000,
  JW_FMT_Au64 = 0x2000,
  JW_FMT_Bu32 = 0x4000,
  JW_FMT_Bu64 = 0x8000,
};

typedef enum _JournalReadFlags  JournalReadFlags;
typedef enum _JournalWriteFlags JournalWriteFlags;

struct _Integer {
  union {
    struct {
      uint32_t LOW;
      uint32_t HIGH;
    } U32;

    uint64_t U64;
  };
};

struct _Entry {
  ThreadID    tid;
  uint32_t    flags;
  Timestamp   time;
  const char *msg;
  Integer     int1;
  Integer     int2;
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

SUPRUGLUE_DEFINE_LIST_INLINE(BlockList, Block, list);

void JournalInit(Journal *jl);
int  JournalRead(Journal *jl, Entry *record, JournalReadFlags flags);

Entry *getEntry(Journal *jl);
void   setEntry(Journal *jl, Entry *entry);

#define PRULOG_2u32(level, fmt, arg1, arg2)                                                                            \
  do {                                                                                                                 \
    Entry *entry = getEntry(&__system.journal);                                                                        \
    entry->tid = TID(__system_current);                                                                                \
    entry->flags = JW_##level | JW_FMT_Au32 | JW_FMT_Bu32;                                                             \
    entry->msg = (fmt);                                                                                                \
    entry->int1.U32.LOW = (arg1);                                                                                      \
    entry->int2.U32.LOW = (arg2);                                                                                      \
    setEntry(&__system.journal, entry);                                                                                \
  } while (0)

#define PRULOG_1u32(level, fmt, arg1)                                                                                  \
  do {                                                                                                                 \
    Entry *entry = getEntry(&__system.journal);                                                                        \
    entry->tid = TID(__system_current);                                                                                \
    entry->flags = JW_##level | JW_FMT_Au32;                                                                           \
    entry->msg = (fmt);                                                                                                \
    entry->int1.U32.LOW = (arg1);                                                                                      \
    setEntry(&__system.journal, entry);                                                                                \
  } while (0)

#define PRULOG_1u64(level, fmt, arg1)                                                                                  \
  do {                                                                                                                 \
    Entry *entry = getEntry(&__system.journal);                                                                        \
    entry->tid = TID(__system_current);                                                                                \
    entry->flags = JW_##level | JW_FMT_Au64;                                                                           \
    entry->msg = (fmt);                                                                                                \
    entry->int1.U64 = (arg1);                                                                                          \
    setEntry(&__system.journal, entry);                                                                                \
  } while (0)

#define PRULOG_2u64(level, fmt, arg1, arg2)                                                                            \
  do {                                                                                                                 \
    Entry *entry = getEntry(&__system.journal);                                                                        \
    entry->tid = TID(__system_current);                                                                                \
    entry->flags = JW_##level | JW_FMT_Au64 | JW_FMT_Bu64;                                                             \
    entry->msg = (fmt);                                                                                                \
    entry->int1.U64 = (arg1);                                                                                          \
    entry->int2.U64 = (arg2);                                                                                          \
    setEntry(&__system.journal, entry);                                                                                \
  } while (0)

#define PRULOG_0(level, fmt)                                                                                           \
  do {                                                                                                                 \
    Entry *entry = getEntry(&__system.journal);                                                                        \
    entry->tid = TID(__system_current);                                                                                \
    entry->flags = JW_##level;                                                                                         \
    entry->msg = (fmt);                                                                                                \
    setEntry(&__system.journal, entry);                                                                                \
  } while (0)

#define METLOG_2u64(th, fmt, arg1, arg2)                                                                               \
  do {                                                                                                                 \
    Entry *entry = getEntry(&__system.journal);                                                                        \
    entry->tid = TID(th);                                                                                              \
    entry->flags = JW_METRICS | JW_FMT_Au64 | JW_FMT_Bu64;                                                             \
    entry->msg = (fmt);                                                                                                \
    entry->int1.U64 = (arg1);                                                                                          \
    entry->int2.U64 = (arg2);                                                                                          \
    setEntry(&__system.journal, entry);                                                                                \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif // LIB_LOG_JOURNAL_JOURNAL_H
