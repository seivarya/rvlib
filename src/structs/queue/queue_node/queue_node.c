/* <queue_node.c>: queue node methods */

#include "queue_node.h"
#include <stdio.h>
#include <string.h>

queue_node *queue_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[queue_node:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	queue_node *node = malloc(sizeof(queue_node));
	if (!node) {
		perror("malloc failed: queue_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[queue_node:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in queue_node_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void queue_node_destruct(queue_node *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <queue_node.c> */
