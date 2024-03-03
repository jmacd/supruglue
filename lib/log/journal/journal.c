// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/log/journal/journal.h"
#include "lib/debug/debug.h"
#include "lib/sync/sync.h"
#include "lib/time/clock.h"
#include <stdio.h>
#include <string.h>

SUPRUGLUE_DEFINE_LIST(BlockList, Block, list);

const char *const overflowMessage = "** OVERFLOW **: dropped %u records";

void JournalInit(Journal *jl) {
  memset(jl, 0, sizeof(*jl));
  LockInit(&jl->lock);
  BlockListInit(&jl->data);
  BlockListInit(&jl->free);
  int i;
  for (i = 0; i < NUM_BLOCKS; i++) {
    BlockListPushBack(&jl->free, &jl->blocks[i]);
  }
}

int JournalRead(Journal *jl, Entry *record, JournalReadFlags flags) {
  if (BlockListEmpty(&jl->data)) {
    if ((flags & JR_BLOCKING) == 0) {
      return -1;
    }
    SemaDown(&jl->lock);
  }

  Block *block = BlockListFront(&jl->data);
  *record = block->entries[block->cursor++];

  if (block->cursor != block->count) {
    return 0;
  }
  BlockListRemove(block);
  block->count = 0;
  block->cursor = 0;
  BlockListPushBack(&jl->free, block);
  return 0;
}

Entry *getEntry(Journal *jl) {
  Block *block;
  // Empty case: add the first block.
  if (BlockListEmpty(&jl->data)) {
    block = BlockListPopFront(&jl->free);
    BlockListPushBack(&jl->data, block);
  } else {
    block = BlockListBack(&jl->data);
  }

  // If the first block is full and there is a free block, use it.
  if (block->count == NUM_PER_BLOCK && !BlockListEmpty(&jl->free)) {
    block = BlockListPopFront(&jl->free);
    BlockListPushBack(&jl->data, block);
  }

  // If the block has room, use it.
  if (block->count < NUM_PER_BLOCK) {
    return &block->entries[block->count++];
  }

  // Recycle the previous block from the back of the block list.
  Block *swap = BlockListPrev(block);
  BlockListRemove(swap);
  BlockListPushBack(&jl->data, swap);

  int32_t loss = NUM_PER_BLOCK + 1;
  // If an overflow record is being lost, retain its loss count.
  if (swap->entries[0].msg == overflowMessage) {
    loss += swap->entries[0].int1.U32.LOW - 1;
  }

  // Block is now the second-newest record, and its entry[0] is the
  // oldest entry after a new gap in the log.
  Entry *entry = &block->entries[0];
  entry->tid = OVERFLOW_THREAD_ID;
  entry->msg = overflowMessage;
  entry->int1.U32.LOW = loss;
  entry->int2.U32.LOW = 0;

  // Swap is the newest record.
  swap->count = 1;
  swap->cursor = 0;
  return &swap->entries[0];
}

void setEntry(Journal *jl, Entry *entry) {
  ReadClock(&entry->time);

  SemaUp(&jl->lock);

  if ((entry->flags & JW_YIELD) != 0) {
    Yield();
  }
}
