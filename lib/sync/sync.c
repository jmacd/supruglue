// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "sync.h"

void LockInit(LockWord *word) {
  ThreadListInit(&word->waiters);
}

void LockAwait(LockWord *word) {
  ThreadListPushBack(&word->waiters, __system_current);
  YieldBlocked();
}

void LockWake(LockWord *word) {
  while (!ThreadListEmpty(&word->waiters)) {
    Thread *wake = ThreadListPopFront(&word->waiters);
    ThreadListPushFront(&__system_runnable, wake);
  }
}

void SemaphoreInit(Semaphore *semaphore) {
  LockInit(&semaphore->lock);
  semaphore->value = 0;
}

void SemaphoreUp(Semaphore *semaphore) {
  if (!ThreadListEmpty(&semaphore->lock.waiters)) {
    Thread *wake = ThreadListPopFront(&semaphore->lock.waiters);
    ThreadListPushFront(&__system_runnable, wake);
    return;
  }
  semaphore->value++;
}

void SemaphoreDown(Semaphore *semaphore) {
  if (semaphore->value > 0) {
    semaphore->value--;
    return;
  }
  LockAwait(&semaphore->lock);
}
