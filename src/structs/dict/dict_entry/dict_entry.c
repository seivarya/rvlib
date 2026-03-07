/* <dict_dict_entry.c>: dictionary dict_entry methods */

#include "dict_entry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dict_entry *dict_entry_construct(const char *key, void *value, const td *type) {
	if (!key) {
		fprintf(stderr, "[dict_entry:construct] key cannot be NULL\n");
		return NULL;
	}

	dict_entry *node = malloc(sizeof(dict_entry));
	if (!node) {
		perror("malloc failed: dict_entry_construct");
		return NULL;
	}

	if (!type || !td_validator(type)) {
		fprintf(stderr, "[dict_entry:construct] invalid type descriptor\n");
		free(node);
		return NULL;
	}

	node->type = type;

	node->key = malloc(strlen(key) + 1);
	if (!node->key) {
		perror("malloc failed for key in dict_entry_construct");
		free(node);
		return NULL;
	}
	strcpy(node->key, key);

	if (type->copy) {
		node->value = type->copy(value);
	} else {
		node->value = malloc(type->size);
		if (!node->value) {
			perror("malloc failed for value in dict_entry_construct");
			free(node->key);
			free(node);
			return NULL;
		}
		memcpy(node->value, value, type->size);
	}

	node->next = NULL;
	return node;
}

lib_status dict_entry_destruct(dict_entry *node) {
	if (!node)
		return LIB_PTR_INVALID;

	if (node->type && node->type->destruct) {
		node->type->destruct(node->value);
	} else {
		free(node->value);
	}

	free(node->key);
	free(node);
	return LIB_OK;
}
/* <dict_entry.c> */
