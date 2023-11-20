#include <stddef.h>
#include <string.h>

#include "lib/args/args.h"

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

const char *ParseFlag(Args *args, Flags *flags, Flag *match) {
  Args save = *args;

  const char *arg = TakeArg(args);
  if (arg == NULL) {
    return NULL;
  }
  const char *find = arg;
  for (; *find != 0 && *find != '='; find++) {
  }
  if (*find == 0) {
    *args = save;
    return NULL;
  }
  const char *key = save.ptr;

  size_t  len = find - key;
  int32_t i = 0;
  for (; i < flags->num_defs; i++) {
    if (strncmp(flags->defs[i].key, key, len) == 0) {
      *match = flags->defs[i];
      return find + 1;
    }
  }
  *args = save;
  return NULL;
}
