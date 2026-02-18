#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rvlib/td.h>

#include "entry.h"


entry* entry_construct(const char* key, void* value, const td *type) {
	if (!key) {
		fprintf(stderr, "entry_construct: key cannot be NULL\n");
		return NULL;
	}

	entry *node = malloc(sizeof(entry));
	if (!node) {
		perror("malloc failed: entry_construct");
		return NULL;
	}

	if (!type || !td_validator(type)) {
		fprintf(stderr, "entry_construct: invalid type descriptor\n");
		free(node);
		return NULL;
	}

	node->type = type;

	node->key = malloc(strlen(key) + 1);
	if (!node->key) {
		perror("malloc failed for key in entry_construct");
		free(node);
		return NULL;
	}
	strcpy(node->key, key);

	if (type->copy) {
		node->value = type->copy(value);
	} else {
		node->value = malloc(type->size);
		if (!node->value) {
			perror("malloc failed for value in entry_construct");
			free(node->key);
			free(node);
			return NULL;
		}
		memcpy(node->value, value, type->size);
	}

	node->next = NULL;
	return node;
}

void entry_destruct(entry *node) {
	if (!node) return;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->value);
	} else {
		free(node->value);
	}

	free(node->key);
	free(node);
}
