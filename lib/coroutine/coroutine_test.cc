// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "absl/strings/str_format.h"
#include "lib/coroutine/coroutine.h"
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

// Note: the overflow test here depends on stack_size=500
SUPRUGLUE_DECLARE_SIZED(test, Thread, thread, uint8_t, 500);

typedef struct {
  SUPRUGLUE_SIZED_TYPENAME(test, Thread, uint8_t) thread;
  uint32_t  index;
  uint32_t *regs;
} TestThread1;

typedef struct {
  SUPRUGLUE_SIZED_TYPENAME(test, Thread, uint8_t) thread;
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

TEST(CoroutineTest, TwoThreads) {
  TestThread1 threads[2];
  uint32_t    regs[2] = {0, 0};

  threads[0].index = 0;
  threads[1].index = 1;
  threads[0].regs = regs;
  threads[1].regs = regs;

  auto test_func = [](ThreadID tid, Args args) {
    TestThread1 *tt = (TestThread1 *)tid;
    tt->regs[tt->index] = tt->index + 1;
  };

  EXPECT_EQ(0, Init(NewSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0].thread.thread, test_func, Args{.ptr = "1"}, "a", sizeof(threads[0].thread.space)));
  EXPECT_EQ(0, Create(&threads[1].thread.thread, test_func, Args{.ptr = "2"}, "b", sizeof(threads[1].thread.space)));

  EXPECT_EQ(0, ::Run());
  EXPECT_EQ(1, regs[0]);
  EXPECT_EQ(2, regs[1]);
}

TEST(CoroutineTest, TwiceAlternating) {
  TestThread1 threads[2];

  EXPECT_EQ(0, Init(NewSystemConfig()));

  EXPECT_EQ(0,
            Create(&threads[0].thread.thread, test_run_func, Args{.ptr = "2"}, "a", sizeof(threads[0].thread.space)));
  EXPECT_EQ(0,
            Create(&threads[1].thread.thread, test_run_func, Args{.ptr = "22"}, "b", sizeof(threads[1].thread.space)));

  EXPECT_EQ(0, ::Run());

  auto id0 = TID(&threads[0].thread.thread);
  auto id1 = TID(&threads[1].thread.thread);

  vector<ThreadID> expect = {
      id0,
      id1,
      id0,
      id1,
  };

  EXPECT_EQ(expect, test_run_get());
}

TEST(CoroutineTest, Overflow) {
  TestThread1 thread;
  int         num = 1;

  for (; num < 20; num++) {
    EXPECT_EQ(0, Init(NewSystemConfig()));

    std::string arg = absl::StrFormat("%d", num);

    EXPECT_EQ(0, Create(&thread.thread.thread, test_recursive_func, Args{.ptr = arg.c_str()}, "abc",
                        sizeof(thread.thread.space)));
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

TEST(CoroutineTest, MultiOverflow) {
  TestThread1 thread0;
  TestThread1 thread1;

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0,
            Create(&thread0.thread.thread, test_recursive_func, Args{.ptr = "20"}, "a", sizeof(thread0.thread.space)));
  EXPECT_EQ(0,
            Create(&thread1.thread.thread, test_recursive_func, Args{.ptr = "20"}, "b", sizeof(thread1.thread.space)));
  EXPECT_EQ(0, ::Run());

  auto logs = test_logs_get();
  EXPECT_EQ(2, logs.size());
  EXPECT_THAT(logs[0], HasSubstr("[a] stack overflow"));
  EXPECT_THAT(logs[1], HasSubstr("[b] stack overflow"));
}
