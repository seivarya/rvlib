/* <dqueue_node.c>: queue node methods */

#include "dqueue_node.h"
#include <stdio.h>
#include <string.h>

dqueue_node *dqueue_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[dqueue_node:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	dqueue_node *node = malloc(sizeof(dqueue_node));
	if (!node) {
		perror("malloc failed: dqueue_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->prev = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[dqueue_node:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in dqueue_node_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void dqueue_node_destruct(dqueue_node *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
} /* <dqueue_node.c> */
