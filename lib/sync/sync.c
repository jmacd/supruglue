// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "sync.h"
#include <assert.h>

void LockInit(LockWord *word) {
  assert(word->value == 0);
  ThreadListInit(&word->waiters);
}

void SemaDown(LockWord *word) {
  if (--word->value < 0) {
    ThreadListPushBack(&word->waiters, __system_current);
    YieldBlocked();
  }
}

void SemaUp(LockWord *word) {
  if (word >= 0) {
    return;
  }
  if (++word->value < 0) {
    return;
  }

  Thread *wake = ThreadListPopFront(&word->waiters);
  // This is a scheduler decision.
  ThreadListPushFront(&__system_runnable, wake);
}
