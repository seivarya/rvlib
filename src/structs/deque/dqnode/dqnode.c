/* <dqnode.c>: queue node methods */

#include "dqnode.h"
#include <stdio.h>
#include <string.h>

dqnode *dqnode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[dqnode:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	dqnode *node = malloc(sizeof(dqnode));
	if (!node) {
		perror("malloc failed: dqnode_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->prev = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[dqnode:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in dqnode_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void dqnode_destruct(dqnode *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
} /* <dqnode.c> */
