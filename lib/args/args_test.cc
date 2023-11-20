// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/args/args.h"
#include "gtest/gtest.h"

TEST(ArgsTest, Atoi) {
  EXPECT_EQ(0, Atoi("0"));
  EXPECT_EQ(0, Atoi("0 "));
  EXPECT_EQ(0, Atoi("0 h"));
  EXPECT_EQ(1, Atoi("1"));
  EXPECT_EQ(10, Atoi("10"));
  EXPECT_EQ(15, Atoi("15"));
  EXPECT_EQ(1501, Atoi("1501"));
  EXPECT_EQ(1501010, Atoi("1501010 and more"));
  EXPECT_EQ(15, Atoi("15 12 10"));
}

const char *const testArgStr = "key=value\0letter=w\0\0";

TEST(ArgsTest, ParseTwoFlags) {
  Args  args{.ptr = testArgStr};
  Flag  match;
  Flag  defs[2];
  Flags flags;
  flags.num_defs = 2;
  flags.defs = defs;
  defs[0].key = "key";
  defs[1].key = "letter";
  EXPECT_STREQ("value", ParseFlag(&args, &flags, &match));
  EXPECT_EQ("key", match.key);

  EXPECT_STREQ("w", ParseFlag(&args, &flags, &match));
  EXPECT_EQ("letter", match.key);

  EXPECT_EQ(nullptr, ParseFlag(&args, &flags, &match));
}

TEST(ArgsTest, ParseFlagsUnknown) {
  Args  args{.ptr = testArgStr};
  Flag  match;
  Flag  defs[1];
  Flags flags;
  flags.num_defs = 1;
  flags.defs = defs;
  defs[0].key = "nodef";

  EXPECT_EQ(nullptr, ParseFlag(&args, &flags, &match));
  EXPECT_EQ(testArgStr, args.ptr);
}

TEST(ArgsTest, ParseFlagsMissing) {
  Args  args{.ptr = "k=v\0other\0\0"};
  Flag  match;
  Flag  defs[1];
  Flags flags;
  flags.num_defs = 1;
  flags.defs = defs;
  defs[0].key = "k";

  EXPECT_STREQ("v", ParseFlag(&args, &flags, &match));
  EXPECT_EQ("k", match.key);

  EXPECT_EQ(nullptr, ParseFlag(&args, &flags, &match));
  EXPECT_STREQ("other", TakeArg(&args));
  EXPECT_EQ(nullptr, TakeArg(&args));
}
