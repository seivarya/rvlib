/* clist_node.h: circular linked list node interface */

#ifndef CLIST_NODE_H
#define CLIST_NODE_H

#include <rvlib/td.h>

typedef struct clist_node clist_node;

struct clist_node {
	void *data;
	clist_node *next;
	const td *type;
	clist_node *previous;
};

clist_node* clist_node_construct(void *data, const td *type);
void clist_node_destruct(clist_node *node);

#endif /* clist_node_h */
