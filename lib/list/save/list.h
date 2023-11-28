// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_LIST_LIST_H
#define LIB_LIST_LIST_H

#include "generic.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Note: The definitions here have been inlined because it reduces
// program size, however another experiment count be done--there are
// currently two list definitions with nearly identical code.  They
// can be abstracted to use the LinkOffset() method as an argument,
// and thus have one definition per method.  With inline functions
// this seems to help, but possibly half as many non-inline functions
// is better.

#define SUPRUGLUE_DECLARE_LIST(LTYPE, ETYPE)                                                                           \
                                                                                                                       \
  typedef struct _##LTYPE LTYPE;                                                                                       \
                                                                                                                       \
  struct _##LTYPE {                                                                                                    \
    LTYPE *next;                                                                                                       \
    LTYPE *prev;                                                                                                       \
  }

#define SUPRUGLUE_DEFINE_LIST_INLINE(LTYPE, ETYPE, LNAME)                                                              \
                                                                                                                       \
  static inline AnyList *LTYPE##_AL(LTYPE *list) { return (AnyList *)list; }                                           \
                                                                                                                       \
  static inline ptrdiff_t LTYPE##_LO() { return (ptrdiff_t) & ((ETYPE *)0)->LNAME; }                                   \
                                                                                                                       \
  static inline void LTYPE##Init(LTYPE *list) { AnyListInit(LTYPE##_AL(list)); }                                       \
                                                                                                                       \
  static inline void LTYPE##Delete(LTYPE *next, LTYPE *prev) { AnyListDelete(LTYPE##_AL(next), LTYPE##_AL(prev)); }    \
                                                                                                                       \
  static inline void LTYPE##Add(LTYPE *prev, LTYPE *next, LTYPE *insert) {                                             \
    AnyListAdd(LTYPE##_AL(prev), LTYPE##_AL(next), LTYPE##_AL(insert));                                                \
  }                                                                                                                    \
                                                                                                                       \
  static inline ETYPE *LTYPE##Entry(LTYPE *list) { return (ETYPE *)AnyListEntry(LTYPE##_AL(list), LTYPE##_LO()); }     \
                                                                                                                       \
  static inline int LTYPE##Empty(LTYPE *list) { return AnyListEmpty(LTYPE##_AL(list)); }                               \
                                                                                                                       \
  static inline ETYPE *LTYPE##Next(ETYPE *item) { return (ETYPE *)AnyListNext(item, LTYPE##_LO()); }                   \
                                                                                                                       \
  static inline ETYPE *LTYPE##Prev(ETYPE *item) { return (ETYPE *)AnyListPrev(item, LTYPE##_LO()); }                   \
                                                                                                                       \
  static inline ETYPE *LTYPE##Front(LTYPE *list) { return (ETYPE *)AnyListFront(LTYPE##_AL(list), LTYPE##_LO()); }     \
                                                                                                                       \
  static inline ETYPE *LTYPE##End(LTYPE *list) { return (ETYPE *)AnyListEnd(LTYPE##_AL(list), LTYPE##_LO()); }         \
                                                                                                                       \
  static inline ETYPE *LTYPE##Back(LTYPE *list) { return (ETYPE *)AnyListBack(LTYPE##_AL(list), LTYPE##_LO()); }       \
                                                                                                                       \
  static inline void LTYPE##Remove(ETYPE *item) { AnyListRemove(item, LTYPE##_LO()); }                                 \
                                                                                                                       \
  static inline ETYPE *LTYPE##PopFront(LTYPE *list) {                                                                  \
    return (ETYPE *)AnyListPopFront(LTYPE##_AL(list), LTYPE##_LO());                                                   \
  }                                                                                                                    \
                                                                                                                       \
  static inline ETYPE *LTYPE##PopBack(LTYPE *list) { return (ETYPE *)AnyListPopBack(LTYPE##_AL(list), LTYPE##_LO()); } \
                                                                                                                       \
  static inline void LTYPE##PushBack(LTYPE *list, ETYPE *item) {                                                       \
    AnyListPushBack(LTYPE##_AL(list), item, LTYPE##_LO());                                                             \
  }                                                                                                                    \
                                                                                                                       \
  static inline void LTYPE##PushFront(LTYPE *list, ETYPE *item) {                                                      \
    AnyListPushFront(LTYPE##_AL(list), item, LTYPE##_LO());                                                            \
  }

#define SUPRUGLUE_DEFINE_LIST(LTYPE, ETYPE, LNAME)

#ifdef __cplusplus
}
#endif

#endif // LIB_LIST_LIST_H
