/* clist.h: circular linked list header file */

#ifndef CLIST_H
#define CLIST_H

#include "clist/clist_node/clist_node.h"
#include "td.h"
#include <rvlib/err.h>

typedef struct clist {
	clist_node *head;
	clist_node *tail;
	size_t length;
} clist;

clist *clist_construct(void);
lib_status clist_destruct(clist *list);

lib_status clist_insert(clist *list, size_t index, void *data, const td *type);
lib_status clist_remove(clist *list, size_t index);
lib_status clist_print(clist *list);

void *clist_fetch_node(clist *list, size_t index);

#endif /* clist_h */
