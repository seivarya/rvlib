/* stack_node.h: stack node interface */

#ifndef STACK_NODE_H
#define STACK_NODE_H

#include <stdlib.h>
#include <rvlib/td.h> 

typedef struct stack_node stack_node;

struct stack_node {
	void *data;
	const td *type;
	stack_node *next;
};

stack_node* stack_node_construct(void *data, const td *type);
void stack_node_destruct(stack_node *node);

#endif /* stack_node_h */
