// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/log/journal/journal.h"

SUPRUGLUE_DEFINE_LIST(EntryList, Entry, list);

void JournalInit(Journal *jl) {
  EntryListInit(&jl->pending);
  EntryListInit(&jl->freelist);
  for (int i = 0; i < NUM_JOURNAL_ENTRIES; i++) {
    EntryListPushBack(&jl->freelist, &jl->space[i]);
  }
}

int JournalRead(Journal *jl, Entry *record) {
  if (EntryListEmpty(&jl->pending)) {
    return -1;
  }
  Entry *first = EntryListPopFront(&jl->pending);
  *record = *first;
  EntryListPushBack(&jl->freelist, first);
  return 0;
}

void collapse(Journal *jl) {
  //   ent->msg = "** OVERFLOW **: dropped %u records";
  Entry *p = EntryListFront(&jl->pending);

  for (int i = 0; i < (NUM_JOURNAL_ENTRIES / 2) - 1; i++) {
    p = EntryListNext(p);
  }
}

void JournalWrite(Journal *jl, ThreadID tid, const char *msg, int32_t arg1, int32_t arg2) {
  if (EntryListEmpty(&jl->freelist)) {
    collapse(jl);
  }
  Entry *fill = EntryListPopFront(&jl->freelist);
  fill->msg = msg;
  fill->arg1 = arg1;
  fill->arg2 = arg2;
  fill->tid = tid;
  EntryListPushBack(&jl->pending, fill);
}
