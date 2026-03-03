/* dlist_node.h: doubly linked list node interface */

#ifndef DLIST_NODE_H
#define DLIST_NODE_H

#include <stdlib.h>
#include <rvlib/td.h>

typedef struct dlist_node dlist_node;

struct dlist_node {
	void *data;
	dlist_node *next;
	const td *type;
	dlist_node *previous;
};

dlist_node* dlist_node_construct(void *data, const td *type);
void dlist_node_destruct(dlist_node *node);

#endif /* dlist_node_h */
