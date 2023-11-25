// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "generic.h"

void AnyListInit(AnyList *l) {
  l->next = l;
  l->prev = l;
}

void AnyListDelete(AnyList *next, AnyList *prev) {
  next->prev = prev;
  prev->next = next;
}

void *AnyListEntry(AnyList *list, ptrdiff_t offset) {
  return (void *)(((uint8_t *)list) - offset);
}

AnyList *AnyListAtOffset(void *item, ptrdiff_t offset) {
  return (AnyList *)(((uint8_t *)item) + offset);
}

int AnyListEmpty(AnyList *list) {
  return list == list->next;
}

void *AnyListFront(AnyList *list, ptrdiff_t offset) {
  return AnyListEntry(list->next, offset);
}

void *AnyListBack(AnyList *list, ptrdiff_t offset) {
  return AnyListEntry(list->prev, offset);
}

void *AnyListNext(void *item, ptrdiff_t offset) {
  return AnyListFront(AnyListAtOffset(item, offset), offset);
}

void *AnyListPrev(void *item, ptrdiff_t offset) {
  return AnyListBack(AnyListAtOffset(item, offset), offset);
}

int AnyListEnd(AnyList *list, void *item, ptrdiff_t offset) {
  return list == AnyListAtOffset(item, offset);
}

void AnyListPushBack(AnyList *list, void *item, ptrdiff_t offset) {
  AnyListAdd(list->prev, list, AnyListAtOffset(item, offset));
}

void AnyListPushFront(AnyList *list, void *item, ptrdiff_t offset) {
  AnyListAdd(list, list->next, AnyListAtOffset(item, offset));
}

void AnyListRemove(void *item, ptrdiff_t offset) {
  AnyList *list = AnyListAtOffset(item, offset);
  AnyListDelete(list->next, list->prev);
}

void AnyListAdd(AnyList *prev, AnyList *next, AnyList *ins) {
  next->prev = ins;
  prev->next = ins;
  ins->next = next;
  ins->prev = prev;
}

void *AnyListPopFront(AnyList *list, ptrdiff_t offset) {
  AnyList *next = list->next;
  AnyListDelete(next->next, list);
  return AnyListEntry(next, offset);
}

void *AnyListPopBack(AnyList *list, ptrdiff_t offset) {
  AnyList *prev = list->prev;
  AnyListDelete(list, prev->prev);
  return AnyListEntry(prev, offset);
}
