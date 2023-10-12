#include "lib/cpx/cpx.h"
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>

void Thread1(ThreadID thid, Args args) {
  while (1) {
    printf("arg %s\n", args.ptr);
    Yield();
  }
}

int main() {
  Args    args1 = {.ptr = "1"};
  Args    args2 = {.ptr = "2"};
  Thread  th1;
  Thread  th2;
  uint8_t stack1[1024];
  uint8_t stack2[1024];

  Init(NewSystemConfig());

  Create(&th1, Thread1, args1, NewThreadConfig("a", stack1, 1024));
  Create(&th2, Thread1, args2, NewThreadConfig("b", stack2, 1024));

  return Run();
}
