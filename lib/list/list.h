// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_LIST_LIST_H
#define LIB_LIST_LIST_H

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
  };

#define SUPRUGLUE_DEFINE_LIST_INLINE(LTYPE, ETYPE, LNAME)                                                              \
                                                                                                                       \
  static inline void LTYPE##Init(LTYPE *l) {                                                                           \
    l->next = l;                                                                                                       \
    l->prev = l;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
  static inline void LTYPE##Delete(LTYPE *next, LTYPE *prev) {                                                         \
    next->prev = prev;                                                                                                 \
    prev->next = next;                                                                                                 \
  }                                                                                                                    \
                                                                                                                       \
  static inline void LTYPE##Add(LTYPE *prev, LTYPE *next, LTYPE *ins) {                                                \
    next->prev = ins;                                                                                                  \
    prev->next = ins;                                                                                                  \
    ins->next = next;                                                                                                  \
    ins->prev = prev;                                                                                                  \
  }                                                                                                                    \
                                                                                                                       \
  static inline ptrdiff_t LTYPE##LinkOffset() { return (ptrdiff_t) & ((ETYPE *)0)->LNAME; }                            \
                                                                                                                       \
  static inline ETYPE *LTYPE##Entry(LTYPE *l) { return (ETYPE *)((uint8_t *)l - LTYPE##LinkOffset()); }                \
                                                                                                                       \
  static inline int LTYPE##Empty(LTYPE *l) { return l == l->next; }                                                    \
                                                                                                                       \
  static inline ETYPE *LTYPE##Next(ETYPE *f) { return LTYPE##Entry(f->LNAME.next); }                                   \
                                                                                                                       \
  static inline ETYPE *LTYPE##Prev(ETYPE *f) { return LTYPE##Entry(f->LNAME.prev); }                                   \
                                                                                                                       \
  static inline ETYPE *LTYPE##Front(LTYPE *f) { return LTYPE##Entry(f->next); }                                        \
                                                                                                                       \
  static inline ETYPE *LTYPE##Back(LTYPE *f) { return LTYPE##Entry(f->prev); }                                         \
                                                                                                                       \
  static inline int LTYPE##End(LTYPE *f, ETYPE *i) { return f == &i->LNAME; }                                          \
                                                                                                                       \
  static inline void LTYPE##PushBack(LTYPE *l, ETYPE *i) { LTYPE##Add(l->prev, l, &i->LNAME); }                        \
                                                                                                                       \
  static inline void LTYPE##Remove(ETYPE *f) { LTYPE##Delete(f->LNAME.next, f->LNAME.prev); }                          \
                                                                                                                       \
  static inline ETYPE *LTYPE##PopFront(LTYPE *l) {                                                                     \
    LTYPE *i = l->next;                                                                                                \
    LTYPE##Delete(i->next, l);                                                                                         \
    return LTYPE##Entry(i);                                                                                            \
  }                                                                                                                    \
                                                                                                                       \
  static inline ETYPE *LTYPE##PopBack(LTYPE *l) {                                                                      \
    LTYPE *i = l->prev;                                                                                                \
    LTYPE##Delete(l, i->prev);                                                                                         \
    return LTYPE##Entry(i);                                                                                            \
  }                                                                                                                    \
                                                                                                                       \
  static inline void LTYPE##PushFront(LTYPE *l, ETYPE *i) { LTYPE##Add(l, l->next, &i->LNAME); }

#define SUPRUGLUE_DEFINE_LIST(LTYPE, ETYPE, LNAME)

#ifdef __cplusplus
}
#endif

#endif // LIB_LIST_LIST_H
