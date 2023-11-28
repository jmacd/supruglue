// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_LIST_GENERIC_H
#define LIB_LIST_GENERIC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AnyList AnyList;

struct _AnyList {
  AnyList *next;
  AnyList *prev;
};

void     AnyListInit(AnyList *l);
AnyList *AnyListAtOffset(void *item, ptrdiff_t offset);
int      AnyListEmpty(AnyList *list);
void     AnyListAdd(AnyList *prev, AnyList *next, AnyList *ins);
void     AnyListDelete(AnyList *next, AnyList *prev);
void     AnyListPushBack(AnyList *list, void *item, ptrdiff_t offset);
void     AnyListPushBack(AnyList *list, void *item, ptrdiff_t offset);
void     AnyListPushFront(AnyList *list, void *item, ptrdiff_t offset);
void     AnyListPushFront(AnyList *list, void *item, ptrdiff_t offset);
void     AnyListRemove(void *item, ptrdiff_t offset);
void    *AnyListBack(AnyList *list, ptrdiff_t offset);
void    *AnyListEntry(AnyList *list, ptrdiff_t offset);
void    *AnyListFront(AnyList *list, ptrdiff_t offset);
void    *AnyListNext(void *item, ptrdiff_t offset);
void    *AnyListPopBack(AnyList *list, ptrdiff_t offset);
void    *AnyListPopFront(AnyList *list, ptrdiff_t offset);
void    *AnyListPrev(void *item, ptrdiff_t offset);

#ifdef __cplusplus
}
#endif

#endif // LIB_LIST_GENERIC_H
