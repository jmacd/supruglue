// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CPX_ARGS_H
#define LIB_CPX_ARGS_H

#ifdef __cplusplus
extern "C" {
#endif

struct _Args;

typedef struct _Args Args;

struct _Args {
  const char *ptr;
};

const char *TakeArg(Args *args);

#ifdef __cplusplus
}
#endif

#endif // LIB_CPX_ARGS_H
