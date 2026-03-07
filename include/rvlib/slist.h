/* slist.h: singly linked list header file */

#ifndef SLIST_H
#define SLIST_H

#include "slist/slist_node/slist_node.h"
#include "td.h"


typedef struct slist {
	slist_node *head;
	size_t length;
} slist;

slist *slist_construct(void);
void slist_destruct(slist *slist);

void slist_insert(slist *list, size_t index, void *data, const td *type);
void slist_remove(slist *list, size_t index);
void slist_print(slist *list);

void *slist_fetch_node(slist *list, size_t index);

#endif /* slist_h */
