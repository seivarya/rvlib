
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qnode.h"
#include <rvlib/td.h>

qnode* qnode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "Fatal Error: %s: Invalid or NULL type descriptor.\n", __func__);
		return NULL;
	}

	qnode *node = malloc(sizeof(qnode));
	if (!node) {
		perror("malloc failed: qnode_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr, "qnode_construct: NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in qnode_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

void qnode_destruct(qnode *node) {
	if (!node) return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
