/* <stack_node.c>: stack node methods */

#include "stack_node.h"
#include <stdio.h>
#include <string.h>

stack_node *stack_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[stack_node:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	stack_node *node = malloc(sizeof(stack_node));
	if (!node) {
		perror("malloc failed: stack_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[stack_node:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in stack_node_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void stack_node_destruct(stack_node *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <stack_node.c> */
