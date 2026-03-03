/* dlist.h: doubly linked list header file */

#ifndef DLIST_H
#define DLIST_H

#include "td.h"
#include "list/dlist/dlist_node/dlist_node.h"

typedef struct dlist {
	dlist_node *head;
	dlist_node *tail;
	size_t length;
} dlist;

dlist* dlist_construct(void);
void dlist_destruct(dlist *list);

void dlist_insert(dlist *list, size_t index, void *data, const td *type);
void dlist_remove(dlist *list, size_t index);
void dlist_print(dlist *list);

void* dlist_fetch_node(dlist *list, size_t index);

#endif /* dlist_h */
