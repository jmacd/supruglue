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
  while (!ThreadListEmpty(&word->waiters)) {
    Thread *wake = ThreadListPopFront(&word->waiters);
    ThreadListPushFront(&__system_runnable, wake);
  }
}
