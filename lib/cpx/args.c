#include <stddef.h>

#include "lib/cpx/args.h"

const char *TakeArg(Args *args) {
  if (*args->ptr == 0) {
    return NULL;
  }
  const char *p = args->ptr;
  while (*p != 0) {
    p++;
  }
  const char *start = args->ptr;
  args->ptr = p + 1;
  return start;
}
