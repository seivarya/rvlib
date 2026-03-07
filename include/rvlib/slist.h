/* slist.h: singly linked list header file */

#ifndef SLIST_H
#define SLIST_H

#include "slist/slist_node/slist_node.h"
#include "td.h"
#include <rvlib/err.h>

typedef struct slist {
	slist_node *head;
	size_t length;
} slist;

slist *slist_construct(void);
lib_status slist_destruct(slist *slist);

lib_status slist_insert(slist *list, size_t index, void *data, const td *type);
lib_status slist_remove(slist *list, size_t index);
lib_status slist_print(slist *list);

slist_node *slist_fetch_node(slist *list, size_t index);

#endif /* slist_h */
