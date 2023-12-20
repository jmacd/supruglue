// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "sync.h"

void LockInit(LockWord *word) {
  ThreadListInit(&word->waiters);
}

void SemaDown(LockWord *word) {
  // solid(2);
  // flash(2);
  // solid(2);

  ThreadListPushBack(&word->waiters, __system_current);
  YieldBlocked();
}

void SemaUp(LockWord *word) {
  // solid(2);
  // flash(3);
  // solid(2);

  while (!ThreadListEmpty(&word->waiters)) {
    Thread *wake = ThreadListPopFront(&word->waiters);
    ThreadListPushFront(&__system_runnable, wake);
  }
}
