/* stack.h: stack header file */

#ifndef STACK_H
#define STACK_H

#include "td.h"
#include "stack/stack_node/stack_node.h"

typedef struct stack {
	struct stack_node *head;
	size_t length;
} stack;

stack* stack_construct(void);
void stack_destruct(stack *stk);

void push(stack *stk, void *data, const td *type);
void pop(stack *stk);
int is_empty(stack *stk);

void* peek(stack *stk);

#endif /* stack_h */
