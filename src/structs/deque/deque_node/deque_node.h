/* deque_node.h: deque node header file */

#ifndef DEQUE_NODE_H
#define DEQUE_NODE_H

#include <rvlib/td.h>

typedef struct deque_node deque_node;

struct deque_node {
	void *data;
	const td *type;
	deque_node *next;
	deque_node *prev;
};

deque_node *deque_node_construct(void *data, const td *type);
void deque_node_destruct(deque_node *node);

#endif /* deque_node_h */
