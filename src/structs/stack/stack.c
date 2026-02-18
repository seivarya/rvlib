/* stack.c: stack methods */

#include <stdlib.h>
#include <stdio.h>
#include <rvlib/td.h>
#include <rvlib/stack.h>

#include "stknode/stknode.h"

/* info: private methods */

static inline int _validate_stack_ptr(stack *stk) {
	if (stk == NULL) {
		fprintf(stderr, "Error: %s: Stack pointer is NULL.\n", __func__);
		return 0;
	}
	return 1;
}

static inline void _validate_stknode_construction(stack *stk, stknode *node) {
	if (!node) {
		stack_destruct(stk);
		exit(3);
	}
}

static inline int _validate_stindex(stack *stk, size_t index) {
	if (stk == NULL) {
		fprintf(stderr, "Error: %s: Stack pointer is NULL for index validation.\n", __func__);
		return 0;
	}
	if (index >= stk->length) {
		fprintf(stderr, "Error: %s: Index %zu out of bounds for stack length %zu.\n", __func__, index, stk->length);
		return 0;
	}
	return 1;
}

/* info: public methods */

stack* stack_construct(void) {
	stack *stk = malloc(sizeof(stack));
	if (stk) {
		stk->head = NULL;
		stk->length = 0;
		return stk;
	}

	fprintf(stderr, "=== error: stack_construct(): malloc failed ===\n");
	return NULL;
}

void stack_destruct(stack *stk) {
	if (!_validate_stack_ptr(stk))
		return;

	/* destroy all nodes */
	stknode *current = stk->head;
	while (current != NULL) {
		stknode *next = current->next;
		stknode_destruct(current);
		current = next;
	}

	free(stk);
} // do stacks even have reverse() method ???

void push(stack *stk, void *data, const td *type) {
	if (!_validate_stack_ptr(stk))
		return;

	stknode *new_node = stknode_construct(data, type);
	_validate_stknode_construction(stk, new_node);

	/* insert at head */
	if (stk->length == 0) {
		stk->head = new_node; /* new_node->next is already NULL */
	} else {
		new_node->next = stk->head;
		stk->head = new_node;
	}

	stk->length++;
}

void pop(stack *stk) {
	if (!_validate_stack_ptr(stk))
		return;
	if (stk->length == 0) {
		fprintf(stderr, "Error: %s: Attempted to pop from an empty stack.\n", __func__);
		return;
	}

	stknode *target = stk->head;

	/* update head */
	if (stk->length == 1) {
		stk->head = NULL;
	} else {
		stk->head = target->next;
	}

	stknode_destruct(target);
	stk->length--;
}

int is_empty(stack *stk) {
	if (!_validate_stack_ptr(stk))
		return 1;

	return stk->length == 0;
}

void* peek(stack *stk) {
	if (!_validate_stack_ptr(stk))
		return NULL;
	if (stk->length == 0) {
		fprintf(stderr, "Error: %s: Attempted to peek from an empty stack.\n", __func__);
		return NULL;
	}

	return stk->head->data;
} /* stack_c */
