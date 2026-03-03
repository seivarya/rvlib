/* <slist_node.c>: singly linked list node methods */

#include "slist_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

slist_node *slist_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[slist_node:construct] invalid type descriptor\n");
		return NULL;
	}

	slist_node *node = malloc(sizeof(slist_node));
	if (!node) {
		perror("malloc failed: slist_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[slist_node:construct] NULL data with no copy function\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in slist_node_construct");
			free(node);
			return NULL;
		}
		memcpy(node->data, data, type->size);
	}

	return node;
}

void slist_node_destruct(slist_node *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <slist_node.c> */
