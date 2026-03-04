/* <deque_node.c>: deque node methods */

#include "deque_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

deque_node *deque_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr,
	  "[deque_node:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	deque_node *node = malloc(sizeof(deque_node));
	if (!node) {
		perror("malloc failed: deque_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->prev = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr,
	   "[deque_node:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in deque_node_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void deque_node_destruct(deque_node *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
} /* <deque_node.c> */
