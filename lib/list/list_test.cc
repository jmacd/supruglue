// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/list/list.h"
#include "gtest/gtest.h"

#include <vector>

using std::vector;

typedef struct _Thing Thing;

SUPRUGLUE_DECLARE_LIST(ThingList, Thing);

struct _Thing {
  int       X;
  ThingList L;
};

SUPRUGLUE_DEFINE_LIST_INLINE(ThingList, Thing, L);

SUPRUGLUE_DEFINE_LIST(ThingList, Thing, L);

vector<int> l2v(ThingList *l) {
  vector<int> v;
  ThingList  *p;
  for (p = l->next; p != l; p = p->next) {
    v.push_back(ThingListEntry(p)->X);
  }

  return v;
}

TEST(ListTest, PushBackRemove) {
  ThingList l;
  ThingListInit(&l);

  EXPECT_EQ(1, ThingListEmpty(&l));

  Thing t1{.X = 1};
  Thing t2{.X = 2};
  Thing t3{.X = 3};
  ThingListPushBack(&l, &t1);
  ThingListPushBack(&l, &t2);
  ThingListPushBack(&l, &t3);

  EXPECT_EQ(0, ThingListEmpty(&l));

  EXPECT_EQ((vector<int>{1, 2, 3}), l2v(&l));
  EXPECT_EQ(1, ThingListFront(&l)->X);
  EXPECT_EQ(3, ThingListBack(&l)->X);

  ThingListRemove(&t2);

  EXPECT_EQ((vector<int>{1, 3}), l2v(&l));
  EXPECT_EQ(1, ThingListFront(&l)->X);
  EXPECT_EQ(3, ThingListBack(&l)->X);

  ThingListRemove(&t1);

  EXPECT_EQ((vector<int>{3}), l2v(&l));
  EXPECT_EQ(3, ThingListFront(&l)->X);
  EXPECT_EQ(3, ThingListBack(&l)->X);

  ThingListRemove(&t3);

  EXPECT_EQ((vector<int>{}), l2v(&l));
  EXPECT_EQ(1, ThingListEmpty(&l));
}

TEST(ListTest, PushFrontPop) {
  ThingList l;
  ThingListInit(&l);

  EXPECT_EQ(1, ThingListEmpty(&l));

  Thing t1{.X = 1};
  Thing t2{.X = 2};
  Thing t3{.X = 3};
  ThingListPushFront(&l, &t1);
  ThingListPushFront(&l, &t2);
  ThingListPushFront(&l, &t3);

  EXPECT_EQ(0, ThingListEmpty(&l));

  EXPECT_EQ((vector<int>{3, 2, 1}), l2v(&l));
  EXPECT_EQ(3, ThingListFront(&l)->X);
  EXPECT_EQ(1, ThingListBack(&l)->X);

  EXPECT_EQ(&t3, ThingListPopFront(&l));

  EXPECT_EQ((vector<int>{2, 1}), l2v(&l));
  EXPECT_EQ(2, ThingListFront(&l)->X);
  EXPECT_EQ(1, ThingListBack(&l)->X);

  EXPECT_EQ(&t1, ThingListPopBack(&l));

  EXPECT_EQ((vector<int>{2}), l2v(&l));
  EXPECT_EQ(2, ThingListFront(&l)->X);
  EXPECT_EQ(2, ThingListBack(&l)->X);

  EXPECT_EQ(&t2, ThingListPopFront(&l));

  EXPECT_EQ((vector<int>{}), l2v(&l));
  EXPECT_EQ(1, ThingListEmpty(&l));
}

TEST(ListTest, Iterate) {
  ThingList l;
  ThingListInit(&l);

  Thing t1{.X = 1};
  Thing t2{.X = 2};
  Thing t3{.X = 3};
  ThingListPushFront(&l, &t1);
  ThingListPushFront(&l, &t2);
  ThingListPushFront(&l, &t3);

  EXPECT_EQ(&t3, ThingListFront(&l));
  EXPECT_EQ(&t2, ThingListNext(ThingListFront(&l)));
  EXPECT_EQ(&t1, ThingListNext(ThingListNext(ThingListFront(&l))));

  EXPECT_EQ(0, ThingListEnd(&l, ThingListFront(&l)));
  EXPECT_EQ(0, ThingListEnd(&l, ThingListNext(ThingListFront(&l))));
  EXPECT_EQ(0, ThingListEnd(&l, ThingListNext(ThingListNext(ThingListFront(&l)))));
  EXPECT_EQ(1, ThingListEnd(&l, ThingListNext(ThingListNext(ThingListNext(ThingListFront(&l))))));

  EXPECT_EQ(&t1, ThingListBack(&l));
  EXPECT_EQ(&t2, ThingListPrev(ThingListBack(&l)));
  EXPECT_EQ(&t3, ThingListPrev(ThingListPrev(ThingListBack(&l))));

  EXPECT_EQ(0, ThingListEnd(&l, ThingListBack(&l)));
  EXPECT_EQ(0, ThingListEnd(&l, ThingListPrev(ThingListBack(&l))));
  EXPECT_EQ(0, ThingListEnd(&l, ThingListPrev(ThingListPrev(ThingListBack(&l)))));
  EXPECT_EQ(1, ThingListEnd(&l, ThingListPrev(ThingListPrev(ThingListPrev(ThingListBack(&l))))));
}
