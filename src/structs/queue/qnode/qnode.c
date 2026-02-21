/* <qnode.c>: queue node methods */

#include "qnode.h"
#include <stdio.h>
#include <string.h>

qnode *qnode_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr, "[qnode:construct] Invalid or NULL type descriptor.\n");
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
			fprintf(stderr, "[qnode:construct] NULL data with no copy function.\n");
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
	if (!node)
		return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
}
/* <qnode.c> */
