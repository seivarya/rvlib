/* stknode.c: stack node methods */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rvlib/td.h>
#include "stknode.h"

stknode* stknode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "Fatal Error: %s: Invalid or NULL type descriptor.\n", __func__);
		return NULL;
	}

	stknode *node = malloc(sizeof(stknode));
	if (!node) {
		perror("malloc failed: stknode_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "stknode_construct: NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in stknode_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void stknode_destruct(stknode *node) {
	if (!node) return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
