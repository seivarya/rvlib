/* <dnode.c>: doubly linked list node methods */

#include "dnode.h"
#include <stdio.h>
#include <string.h>

dnode *dnode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[dnode:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	dnode *node = malloc(sizeof(dnode));
	if (!node) {
		perror("malloc failed: dnode_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->previous = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[dnode:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in dnode_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void dnode_destruct(dnode *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <dnode.c> */
