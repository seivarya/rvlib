#ifndef DQUEUE_NODE_H
#define DQUEUE_NODE_H

#include <stdlib.h>
#include <rvlib/td.h>

typedef struct dqueue_node dqueue_node;

struct dqueue_node {
	void *data;
	const td *type;
	dqueue_node *next;
	dqueue_node *prev;
};

dqueue_node* dqueue_node_construct(void *data, const td *type);
void dqueue_node_destruct(dqueue_node *node);

#endif
