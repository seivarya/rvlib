/* <dlist_node.c>: doubly linked list node methods */

#include "dlist_node.h"
#include <stdio.h>
#include <string.h>

dlist_node *dlist_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[dlist_node:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	dlist_node *node = malloc(sizeof(dlist_node));
	if (!node) {
		perror("malloc failed: dlist_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->previous = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[dlist_node:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in dlist_node_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void dlist_node_destruct(dlist_node *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <dlist_node.c> */
