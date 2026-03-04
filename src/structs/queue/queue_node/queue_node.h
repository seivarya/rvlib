/* queue_node.h: queue node interface */

#ifndef QUEUE_NODE_H
#define QUEUE_NODE_H

#include <rvlib/td.h>

typedef struct queue_node queue_node;

struct queue_node {
	void *data;
	const td *type;
	queue_node *next;
};

queue_node* queue_node_construct(void *data, const td *type);
void queue_node_destruct(queue_node *node);

#endif /* queue_node_h */
