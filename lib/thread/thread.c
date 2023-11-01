// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/thread/thread.h"

SUPRUGLUE_DEFINE_LIST(ThreadList, Thread, list);

ThreadConfig NewThreadConfig(const char *name, uint8_t *stack, size_t stack_size) {
  return (ThreadConfig){
      .nice = 0,
      .stack = stack,
      .stack_size = stack_size,
      .name = name,
  };
}

ThreadID TID(Thread *th) {
  return (ThreadID)th;
}
