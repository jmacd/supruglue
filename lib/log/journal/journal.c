// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/log/journal/journal.h"
#include <stdio.h>
#include <string.h>

SUPRUGLUE_DEFINE_LIST(BlockList, Block, list);

const char *const overflowMessage = "** OVERFLOW **: dropped %u records";

void JournalInit(Journal *jl) {
  memset(jl, 0, sizeof(*jl));
  BlockListInit(&jl->data);
  BlockListInit(&jl->free);
  for (int i = 0; i < NUM_BLOCKS; i++) {
    BlockListPushBack(&jl->free, &jl->blocks[i]);
  }
}

int JournalRead(Journal *jl, Entry *record) {
  if (BlockListEmpty(&jl->data)) {
    return -1;
  }
  Block *block = BlockListFront(&jl->data);
  *record = block->entries[block->cursor++];

  if (block->cursor == block->count) {
    BlockListRemove(block);
    block->count = 0;
    block->cursor = 0;
    BlockListPushBack(&jl->free, block);
  }
  return 0;
}

static Entry *getEntry(Journal *jl) {
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
    loss += swap->entries[0].arg1 - 1;
  }

  // Block is now the second-newest record, and its entry[0] is the
  // oldest entry after a new gap in the log.
  Entry *entry = &block->entries[0];
  entry->tid = 0;
  entry->msg = overflowMessage;
  entry->arg1 = loss;
  entry->arg2 = 0;

  // Swap is the newest record.
  swap->count = 1;
  swap->cursor = 0;
  return &swap->entries[0];
}

void JournalWrite(Journal *jl, ThreadID tid, const char *msg, int32_t arg1, int32_t arg2) {
  Entry *entry = getEntry(jl);
  entry->tid = tid;
  entry->msg = msg;
  entry->arg1 = arg1;
  entry->arg2 = arg2;
}
