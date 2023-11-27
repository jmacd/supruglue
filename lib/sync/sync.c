// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "sync.h"

void LockInit(LockWord *word) {
  ThreadListInit(&word->waiters);
}

void SemaDown(LockWord *word) {
  ThreadListPushBack(&word->waiters, __system_current);
  YieldBlocked();
}

void SemaUp(LockWord *word) {
  if (ThreadListEmpty(&word->waiters)) {
    return;
  }
  Thread *wake = ThreadListPopFront(&word->waiters);
  // This is a scheduler decision.
  ThreadListPushFront(&__system_runnable, wake);
}
