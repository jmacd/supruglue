#include "lib/cpx/cpx_test_c.h"
#include "lib/cpx/args.h"

ThreadID test_run_vector[32];
size_t test_run_vector_size;

void test_run_reset(void) {
  memset(test_run_vector, 0, sizeof(test_run_vector));
  test_run_vector_size = 0;
}

void test_run_func(ThreadID tid, Args args) {
  fprintf(stderr, "enter here\n");
  for (int i = 0; i < 2; i++) {
    fprintf(stderr, "loop body %d\n", i);
    Yield();
    // This follows the Yield so that we test entry and return to the
    // coroutine.
    test_run_vector[test_run_vector_size++] = tid;
  }
  fprintf(stderr, "body return %lu\n", tid);
}

void test_recursive_func(ThreadID tid, Args args) {
  const char *arg = TakeArg(&args);

  fprintf(stderr, "enter\n");
  Yield();
  if (*arg != '0') {
    char two[2];
    two[0] = *arg - 1;
    two[1] = 0;
    Args rargs;
    rargs.ptr = two;
    test_recursive_func(tid, rargs);
  }
  fprintf(stderr, "exit\n");
}
