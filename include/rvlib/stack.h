/* stack.h: stack header file */

#ifndef STACK_H
#define STACK_H

#include "stack/stack_node/stack_node.h"
#include "td.h"
#include <rvlib/err.h>


typedef struct stack {
	struct stack_node *head;
	size_t length;
} stack;

stack *stack_construct(void);
lib_status stack_destruct(stack *stk);

lib_status push(stack *stk, void *data, const td *type);
lib_status pop(stack *stk);
int is_empty(stack *stk);

void *peek(stack *stk);

#endif /* stack_h */
