/* <cnode.c>: circular linked list node methods */

#include "cnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cnode *cnode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[cnode:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	cnode *node = malloc(sizeof(cnode));
	if (!node) {
		perror("malloc failed: cnode_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->previous = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "[cnode:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in cnode_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void cnode_destruct(cnode *node) {
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <cnode.c> */
