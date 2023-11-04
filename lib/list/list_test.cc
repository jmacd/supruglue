// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/list/list.h"
#include "gtest/gtest.h"

typedef struct _Thing Thing;

SUPRUGLUE_DECLARE_LIST(ThingList, Thing);

struct _Thing {
  int       X;
  ThingList L;
};

SUPRUGLUE_DEFINE_LIST_INLINE(ThingList, Thing, L);

SUPRUGLUE_DEFINE_LIST(ThingList, Thing, L);

TEST(ListTest, Basic) {
  ThingList l;
  ThingListInit(&l);

  EXPECT_EQ(0, ThingListLength(&l));

  Thing t1{.X = 1};
  Thing t2{.X = 2};
  Thing t3{.X = 3};
  ThingListPushBack(&l, &t1);
  ThingListPushBack(&l, &t2);
  ThingListPushBack(&l, &t3);

  EXPECT_EQ(3, ThingListLength(&l));

  ThingListRemove(&t2);
  EXPECT_EQ(2, ThingListLength(&l));

  EXPECT_EQ(1, ThingListFront(&l)->X);
  EXPECT_EQ(3, ThingListBack(&l)->X);
}
