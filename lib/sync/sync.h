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
  int32_t    value;
  ThreadList waiters;
};

void LockInit(LockWord *lockword);
void SemaDown(LockWord *lockword);
void SemaUp(LockWord *lockword);

#ifdef __cplusplus
}
#endif

#endif // LIB_SYNC_SYNC_H
