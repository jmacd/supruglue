#include "lib/cpx/cpx_test_c.h"

ThreadID test_run_vector[32];
size_t test_run_vector_size;

void test_run_reset(void) {
  memset(test_run_vector, 0, sizeof(test_run_vector));
  test_run_vector_size = 0;
}

void test_run_func(System *sys, ThreadID arg, const char *args, size_t argsize) {
  fprintf(stderr, "enter here\n");
  for (int i = 0; i < 2; i++) {
    fprintf(stderr, "loop body %d\n", i);
    test_run_vector[test_run_vector_size++] = arg;
    Yield(sys);
  }
  fprintf(stderr, "body return\n");
}
