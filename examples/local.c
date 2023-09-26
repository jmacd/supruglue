#include "lib/cpx/CorePartition.h"
#include <stddef.h>

void Thread1(void *pValue) {
  while (1) {
    printf("pval %p\n", pValue);
    Cpx_Yield();
  }
}

#define NUM_THREADS 2

CpxThread *threads[NUM_THREADS];

int main() {
  // TODO weirdly -- NUM_THREADS was a byte size, so
  // what actually allocates.
  if (Cpx_StaticStart(threads, NUM_THREADS) == false) {
    return (1);
  }

  CpxStaticThread th1, th2;

  bool ok1 = Cpx_CreateStaticThread(Thread1, (void *)1, &th1, 256, 100);
  bool ok2 = Cpx_CreateStaticThread(Thread1, (void *)2, &th2, 256, 200);

  printf("started %d %d\n", ok1, ok2);

  Cpx_Join();

  printf("done!\n");

  return 0;
}
