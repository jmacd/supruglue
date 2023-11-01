// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_LIST_LIST_H
#define LIB_LIST_LIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SUPRUGLUE_DECLARE_LIST(LTYPE, ETYPE)                                                                           \
                                                                                                                       \
  typedef struct _##LTYPE LTYPE;                                                                                       \
                                                                                                                       \
  struct _##LTYPE {                                                                                                    \
    LTYPE *next;                                                                                                       \
    LTYPE *prev;                                                                                                       \
  };                                                                                                                   \
                                                                                                                       \
  ETYPE *LTYPE##Entry(LTYPE *l);                                                                                       \
  void   LTYPE##Init(LTYPE *l);                                                                                        \
  void   LTYPE##Add(LTYPE *prev, LTYPE *next, LTYPE *ins);                                                             \
  void   LTYPE##PushBack(LTYPE *l, ETYPE *i);                                                                          \
  void   LTYPE##Delete(LTYPE *next, LTYPE *prev);                                                                      \
  ETYPE *LTYPE##Remove(ETYPE *f);                                                                                      \
  ETYPE *LTYPE##PopBack(LTYPE *l);                                                                                     \
  ETYPE *LTYPE##PopFront(LTYPE *l);                                                                                    \
  int    LTYPE##Empty(LTYPE *l);                                                                                       \
  ETYPE *LTYPE##Front(LTYPE *f);                                                                                       \
  ETYPE *LTYPE##Back(LTYPE *f);                                                                                        \
  int    LTYPE##End(LTYPE *f, ETYPE *i);                                                                               \
  ETYPE *LTYPE##Next(ETYPE *f);                                                                                        \
  size_t LTYPE##Length(LTYPE *l);

#define SUPRUGLUE_DEFINE_LIST(LTYPE, ETYPE, LNAME)                                                                     \
                                                                                                                       \
  ETYPE *LTYPE##Entry(LTYPE *l) { return (ETYPE *)((char *)l - (ptrdiff_t) & ((ETYPE *)0)->LNAME); }                   \
                                                                                                                       \
  void LTYPE##Init(LTYPE *l) {                                                                                         \
    l->next = l;                                                                                                       \
    l->prev = l;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
  void LTYPE##Add(LTYPE *prev, LTYPE *next, LTYPE *ins) {                                                              \
    next->prev = ins;                                                                                                  \
    prev->next = ins;                                                                                                  \
    ins->next = next;                                                                                                  \
    ins->prev = prev;                                                                                                  \
  }                                                                                                                    \
                                                                                                                       \
  void LTYPE##PushBack(LTYPE *l, ETYPE *i) { LTYPE##Add(l->prev, l, &i->LNAME); }                                      \
                                                                                                                       \
  void LTYPE##Delete(LTYPE *next, LTYPE *prev) {                                                                       \
    next->prev = prev;                                                                                                 \
    prev->next = next;                                                                                                 \
  }                                                                                                                    \
                                                                                                                       \
  ETYPE *LTYPE##Remove(ETYPE *f) {                                                                                     \
    LTYPE *i = f->LNAME.next;                                                                                          \
    LTYPE##Delete(f->LNAME.next, f->LNAME.prev);                                                                       \
    return LTYPE##Entry(i);                                                                                            \
  }                                                                                                                    \
                                                                                                                       \
  ETYPE *LTYPE##PopBack(LTYPE *l) {                                                                                    \
    LTYPE *i = l->prev;                                                                                                \
    LTYPE##Delete(i->next, i->prev);                                                                                   \
    return LTYPE##Entry(i);                                                                                            \
  }                                                                                                                    \
                                                                                                                       \
  ETYPE *LTYPE##PopFront(LTYPE *l) {                                                                                   \
    LTYPE *i = l->next;                                                                                                \
    LTYPE##Delete(i->next, i->prev);                                                                                   \
    return LTYPE##Entry(i);                                                                                            \
  }                                                                                                                    \
                                                                                                                       \
  int LTYPE##Empty(LTYPE *l) { return l == l->next; }                                                                  \
                                                                                                                       \
  ETYPE *LTYPE##Front(LTYPE *f) { return LTYPE##Entry(f->next); }                                                      \
                                                                                                                       \
  ETYPE *LTYPE##Back(LTYPE *f) { return LTYPE##Entry(f->prev); }                                                       \
                                                                                                                       \
  int LTYPE##End(LTYPE *f, ETYPE *i) { return f == &i->LNAME; }                                                        \
                                                                                                                       \
  ETYPE *LTYPE##Next(ETYPE *f) { return LTYPE##Entry(f->LNAME.next); }                                                 \
                                                                                                                       \
  size_t LTYPE##Length(LTYPE *l) {                                                                                     \
    LTYPE *p;                                                                                                          \
    size_t c = 0;                                                                                                      \
                                                                                                                       \
    for (p = l->next; p != l; p = p->next) {                                                                           \
      c += 1;                                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    return c;                                                                                                          \
  }

#ifdef __cplusplus
}
#endif

#endif // LIB_LIST_LIST_H
