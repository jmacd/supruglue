// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/log/journal/journal.h"
#include "gtest/gtest.h"

#define TOTAL (NUM_BLOCKS * NUM_PER_BLOCK)
#define HALF ((NUM_BLOCKS * NUM_PER_BLOCK) >> 1)
#define QUARTER ((NUM_BLOCKS * NUM_PER_BLOCK) >> 2)

void testWrite(Journal *jl, int32_t o, int32_t n) {
  for (int32_t i = o; i < n; i++) {
    JournalWrite(jl, i, "nice journal", i, i + 1, JW_NONE);
  }
}

void testRead(Journal *jl, int32_t i) {
  Entry entry;
  EXPECT_EQ(0, JournalRead(jl, &entry, JR_NONE));
  EXPECT_EQ(i, entry.tid);
  EXPECT_STREQ("nice journal", entry.msg);
  EXPECT_EQ(i, entry.arg1);
  EXPECT_EQ(i + 1, entry.arg2);
}

void testReadOverflow(Journal *jl, int32_t i) {
  Entry entry;
  EXPECT_EQ(0, JournalRead(jl, &entry, JR_NONE));
  EXPECT_EQ(OVERFLOW_THREAD_ID, entry.tid);
  EXPECT_STREQ(overflowMessage, entry.msg);
  EXPECT_EQ(i, entry.arg1);
  EXPECT_EQ(0, entry.arg2);
}

TEST(Journal, EmptyRead) {
  Journal jl;
  JournalInit(&jl);
  Entry entry;
  EXPECT_EQ(-1, JournalRead(&jl, &entry, JR_NONE));
}

TEST(Journal, WriteRead) {
  Journal jl;
  JournalInit(&jl);
  testWrite(&jl, 0, TOTAL);

  for (int32_t i = 0; i < TOTAL; i++) {
    testRead(&jl, i);
  }
  Entry entry;
  EXPECT_EQ(-1, JournalRead(&jl, &entry, JR_NONE));
}

TEST(Journal, OneOverflow) {
  Journal jl;
  JournalInit(&jl);
  testWrite(&jl, 0, TOTAL + 1);

  // First half is intact
  for (int32_t i = 0; i < HALF; i++) {
    testRead(&jl, i);
  }
  // The overflow,
  testReadOverflow(&jl, QUARTER + 1);
  // followed by all but one from the next block.
  for (int32_t i = 1; i < QUARTER; i++) {
    testRead(&jl, HALF + QUARTER + i);
  }
  // Followed by a singleton on the final block.
  testRead(&jl, TOTAL);
  Entry entry;
  EXPECT_EQ(-1, JournalRead(&jl, &entry, JR_NONE));
}

TEST(Journal, RepeatOverflow) {
  for (int writes = 2; writes <= 16; writes++) {
    for (int reads = 1; reads < writes; reads++) {
      // 2 dimensional test
      Journal jl;
      JournalInit(&jl);
      const int repeat = 3;

      int counted = 0;
      for (int i = 0; i < TOTAL * repeat; i++) {
        // write so many
        for (int w = 0; w < writes; w++) {
          JournalWrite(&jl, 2 * i, "repeat", 0, 0, JW_NONE);
        }
        // read so many (fewer)
        for (int r = 0; r < reads; r++) {
          Entry entry;
          EXPECT_EQ(0, JournalRead(&jl, &entry, JR_NONE));
          if (entry.msg == overflowMessage) {
            counted += entry.arg1;
          } else {
            counted += 1;
          }
        }
      }

      // test the total count
      for (int32_t i = 0; i < TOTAL; i++) {
        Entry entry;
        if (JournalRead(&jl, &entry, JR_NONE) < 0) {
          break;
        }

        if (entry.msg == overflowMessage) {
          counted += entry.arg1;
        } else {
          counted += 1;
        }
      }
      EXPECT_EQ(writes * repeat * TOTAL, counted);
    }
  }
}
