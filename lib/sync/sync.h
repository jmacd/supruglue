// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SYNC_SYNC_H
#define LIB_SYNC_SYNC_H

#include "lib/thread/thread.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _LockWord;

typedef struct _LockWord LockWord;

struct _LockWord {
  ThreadList waiters;
};

void LockInit(LockWord *lockword);
void LockAwait(LockWord *lockword);
void LockWake(LockWord *lockword);

struct _Semaphore;

typedef struct _Semaphore Semaphore;

struct _Semaphore {
  LockWord lock;
  int32_t  value;
};

void SemaphoreInit(Semaphore *semaphore);
void SemaphoreUp(Semaphore *semaphore);
void SemaphoreDown(Semaphore *semaphore);

#ifdef __cplusplus
}
#endif

#endif // LIB_SYNC_SYNC_H
