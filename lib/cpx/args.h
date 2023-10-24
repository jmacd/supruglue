// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CPX_ARGS_H
#define LIB_CPX_ARGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Args  Args;
typedef struct _Flags Flags;
typedef struct _Flag  Flag;

struct _Args {
  const char *ptr;
};

struct _Flag {
  const char *key;
};

struct _Flags {
  int32_t     num_defs;
  const Flag *defs;
};

const char *TakeArg(Args *args);
const char *ParseFlag(Args *args, Flags *flags, Flag *match);

int32_t Atoi(const char *arg);

#ifdef __cplusplus
}
#endif

#endif // LIB_CPX_ARGS_H
