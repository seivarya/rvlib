/* snode.c: singly linked list node methods */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rvlib/td.h>
#include "snode.h"

snode* snode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "snode_construct: invalid type descriptor\n");
		return NULL;
	}

	snode *node = malloc(sizeof(snode));
	if (!node) {
		perror("malloc failed: snode_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "snode_construct: NULL data with no copy function\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in snode_construct");
			free(node);
			return NULL;
		}
		memcpy(node->data, data, type->size);
	}

	return node;
}

void snode_destruct(snode *node) {
	if (!node) return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
