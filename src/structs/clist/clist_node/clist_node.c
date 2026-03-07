/* <clist_node.c>: circular linked list node methods */

#include "clist_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

clist_node *clist_node_construct(void *data, const td *type) {
	if (!type || !td_validator(type)) {
		fprintf(stderr,
	  "[clist_node:construct] Invalid or NULL type descriptor.\n");
		return NULL;
	}

	clist_node *node = malloc(sizeof(clist_node));
	if (!node) {
		perror("malloc failed: clist_node_construct");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->previous = NULL;

	if (type->copy) {
		node->data = type->copy(data);
	} else {
		if (!data) {
			fprintf(stderr,
	   "[clist_node:construct] NULL data with no copy function.\n");
			free(node);
			return NULL;
		}

		node->data = malloc(type->size);
		if (!node->data) {
			perror("malloc failed for data in clist_node_construct");
			free(node);
			return NULL;
		}

		memcpy(node->data, data, type->size);
	}

	return node;
}

lib_status clist_node_destruct(clist_node *node) {
	if (!node)
		return LIB_PTR_INVALID;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->data);
	} else {
		free(node->data);
	}

	free(node);
	return LIB_OK;
} /* <clist_node.c> */
