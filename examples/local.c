#include "lib/coroutine/coroutine.h"
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>

void Thread1(ThreadID thid, Args args) {
  while (1) {
    printf("arg %s\n", args.ptr);
    Yield();
  }
}

SUPRUGLUE_DEFINE_THREAD(th1, 256);
SUPRUGLUE_DEFINE_THREAD(th2, 256);

int main() {
  Args args1 = {.ptr = "1"};
  Args args2 = {.ptr = "2"};

  Init(NewSystemConfig());

  Create(&th1.thread, Thread1, args1, "th1", sizeof(th1.space));
  Create(&th2.thread, Thread1, args2, "th2", sizeof(th2.space));

  return Run();
}
