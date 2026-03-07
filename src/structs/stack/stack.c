/* <stack.c>: stack methods */

#include <rvlib/stack.h>
#include <stdio.h>
#include <stdlib.h>


/* info: private methods */

static inline lib_status _validate_stack_ptr(stack *stk) {
	if (stk == NULL) {
		fprintf(stderr, "[stack:validate_stack_ptr] Stack pointer is NULL.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_stack_node_construction(stack *stk,
							   stack_node *node) {
	if (!node) {
		stack_destruct(stk);
		return LIB_ERR_MALLOC;
	}
	return LIB_OK;
}

static inline lib_status _validate_stindex(stack *stk, size_t index) {
	if (stk == NULL) {
		fprintf(stderr, "[stack:validate_stindex] Stack pointer is NULL for index "
	  "validation.\n");
		return LIB_PTR_INVALID;
	}
	if (index >= stk->length) {
		fprintf(stderr,
	  "[stack:validate_stindex] Index %zu out of bounds for stack length "
	  "%zu.\n",
	  index, stk->length);
		return LIB_INDEX_ERR;
	}
	return LIB_OK;
}

/* info: public methods */

stack *stack_construct(void) {
	stack *stk = malloc(sizeof(stack));
	if (stk) {
		stk->head = NULL;
		stk->length = 0;
		return stk;
	}

	fprintf(stderr, "[stack:construct] malloc failed\n");
	return NULL;
}

lib_status stack_destruct(stack *stk) {
	if (_validate_stack_ptr(stk) != LIB_OK)
		return LIB_PTR_INVALID;

	/* destroy all nodes */
	stack_node *current = stk->head;
	while (current != NULL) {
		stack_node *next = current->next;
		stack_node_destruct(current);
		current = next;
	}

	free(stk);
	return LIB_OK;
} // do stacks even have reverse() method ???

lib_status push(stack *stk, void *data, const td *type) {
	if (_validate_stack_ptr(stk) != LIB_OK)
		return LIB_PTR_INVALID;

	stack_node *new_node = stack_node_construct(data, type);
	lib_status c_status = _validate_stack_node_construction(stk, new_node);
	if (c_status != LIB_OK)
		return c_status;

	/* insert at head */
	if (stk->length == 0) {
		stk->head = new_node; /* new_node->next is already NULL */
	} else {
		new_node->next = stk->head;
		stk->head = new_node;
	}

	stk->length++;
	return LIB_OK;
}

lib_status pop(stack *stk) {
	if (_validate_stack_ptr(stk) != LIB_OK)
		return LIB_PTR_INVALID;
	if (stk->length == 0) {
		fprintf(stderr, "[stack:pop] Attempted to pop from an empty stack.\n");
		return LIB_PTR_INVALID;
	}

	stack_node *target = stk->head;

	/* update head */
	if (stk->length == 1) {
		stk->head = NULL;
	} else {
		stk->head = target->next;
	}

	stack_node_destruct(target);
	stk->length--;
	return LIB_OK;
}

int is_empty(stack *stk) {
	if (_validate_stack_ptr(stk) != LIB_OK)
		return 1;

	return stk->length == 0;
}

void *peek(stack *stk) {
	if (_validate_stack_ptr(stk) != LIB_OK)
		return NULL;
	if (stk->length == 0) {
		fprintf(stderr, "[stack:peek] Attempted to peek from an empty stack.\n");
		return NULL;
	}

	return stk->head->data;
} /* <stack.c> */
