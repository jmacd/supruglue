// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CPX_CPX_TEST_C_H
#define LIB_CPX_CPX_TEST_C_H

#include "cpx.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ThreadID test_run_vector[32];
extern size_t test_run_vector_size;

void test_run_reset(void);
void test_run_func(ThreadID arg, const char *args);

#ifdef __cplusplus
}
#endif

#endif // LIB_CPX_CPX_TEST_C_H
