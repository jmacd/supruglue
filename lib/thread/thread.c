// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/thread/thread.h"

SUPRUGLUE_DEFINE_LIST(ThreadList, Thread, list);

SystemYield *__system_yield;
Thread      *__system_current;
ThreadList   __system_runnable;
