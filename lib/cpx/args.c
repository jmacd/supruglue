#include <stddef.h>
#include <stdio.h>

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

int32_t Atoi(const char *p) {
  int32_t r = 0;

  for (; *p != 0; p++) {
    if (*p >= '0' && *p <= '9') {
      r = r * 10;
      r += *p - '0';
      continue;
    }
    break;
  }
  return r;
}
