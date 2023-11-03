// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "absl/strings/str_format.h"
#include "lib/cpx/cpx.h"
#include "lib/log/fmt/fmt.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <unordered_map>

using std::string;
using std::vector;

using testing::HasSubstr;

ThreadID test_run_vector[32];
size_t   test_run_vector_size;

typedef struct {
  Thread    thread;
  uint32_t  index;
  uint32_t *regs;
} TestThread1;

typedef struct {
  Thread         thread;
  vector<string> messages;
} TestThread2;

vector<ThreadID> test_run_get(void) {
  vector<ThreadID> result;
  for (int i = 0; i < test_run_vector_size; i++) {
    result.push_back(test_run_vector[i]);
  }
  memset(test_run_vector, 0, sizeof(test_run_vector));
  test_run_vector_size = 0;
  return result;
}

void test_run_func(ThreadID tid, Args args) {
  for (int i = 0; i < 2; i++) {
    Yield();
    // This follows the Yield so that we test entry and return to the
    // coroutine.
    test_run_vector[test_run_vector_size++] = tid;
  }
}

void test_recursive_func(ThreadID tid, Args args) {
  const char *arg = TakeArg(&args);

  Yield();

  int32_t val = Atoi(arg);
  if (val != 0) {
    Args        rargs;
    std::string sub = absl::StrFormat("%d", val - 1);
    rargs.ptr = sub.c_str();
    test_recursive_func(tid, rargs);
  }
}

vector<string> test_logs_get() {
  vector<string> result;

  Entry ent;
  while (JournalRead(&__system.journal, &ent, JR_NONE) == 0) {
    result.push_back(Format(&ent));
  }
  return result;
}

TEST(CpxTest, TwoThreads) {
  TestThread1 threads[2];
  uint32_t    regs[2] = {0, 0};

  threads[0].index = 0;
  threads[1].index = 1;
  threads[0].regs = regs;
  threads[1].regs = regs;

  uint8_t stack0[1024];
  uint8_t stack1[1024];

  auto test_func = [](ThreadID tid, Args args) {
    TestThread1 *tt = (TestThread1 *)tid;
    tt->regs[tt->index] = tt->index + 1;
  };

  EXPECT_EQ(0, Init(NewSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0].thread, test_func, Args{.ptr = "1"}, NewThreadConfig("a", stack0, 1024)));
  EXPECT_EQ(0, Create(&threads[1].thread, test_func, Args{.ptr = "2"}, NewThreadConfig("b", stack1, 1024)));

  EXPECT_EQ(0, ::Run());
  EXPECT_EQ(1, regs[0]);
  EXPECT_EQ(2, regs[1]);
}

TEST(CpxTest, TwiceAlternating) {
  Thread  threads[2];
  uint8_t stack0[1024];
  uint8_t stack1[1024];

  EXPECT_EQ(0, Init(NewSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0], test_run_func, Args{.ptr = "2"}, NewThreadConfig("a", stack0, 1024)));
  EXPECT_EQ(0, Create(&threads[1], test_run_func, Args{.ptr = "22"}, NewThreadConfig("b", stack1, 1024)));

  EXPECT_EQ(0, ::Run());

  auto id0 = TID(&threads[0]);
  auto id1 = TID(&threads[1]);

  vector<ThreadID> expect = {
      id0,
      id1,
      id0,
      id1,
  };

  EXPECT_EQ(expect, test_run_get());
}

TEST(CpxTest, Overflow) {
  Thread  thread;
  uint8_t stack0[500];
  int     num = 1;

  for (; num < 20; num++) {
    EXPECT_EQ(0, Init(NewSystemConfig()));

    std::string arg = absl::StrFormat("%d", num);

    EXPECT_EQ(0, Create(&thread, test_recursive_func, Args{.ptr = arg.c_str()},
                        NewThreadConfig("abc", stack0, sizeof(stack0))));
    EXPECT_EQ(0, ::Run());

    auto logs = test_logs_get();
    if (logs.size() == 0) {
      continue;
    }
    EXPECT_EQ(1, logs.size());
    EXPECT_THAT(logs[0], HasSubstr("[abc] stack overflow"));
    break;
  }
  EXPECT_LT(1, num);
  EXPECT_GE(10, num);
}

TEST(CpxTest, MultiOverflow) {
  Thread  thread0;
  Thread  thread1;
  uint8_t stack0[500];
  uint8_t stack1[500];

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&thread0, test_recursive_func, Args{.ptr = "20"}, NewThreadConfig("a", stack0, sizeof(stack0))));
  EXPECT_EQ(0, Create(&thread1, test_recursive_func, Args{.ptr = "20"}, NewThreadConfig("b", stack1, sizeof(stack1))));
  EXPECT_EQ(0, ::Run());

  auto logs = test_logs_get();
  EXPECT_EQ(2, logs.size());
  EXPECT_THAT(logs[0], HasSubstr("[a] stack overflow"));
  EXPECT_THAT(logs[1], HasSubstr("[b] stack overflow"));
}
