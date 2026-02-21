#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <rvlib/dict.h>
#include "entry/entry.h"


static inline int _validate_dict_ptr(dict *dictionary) {
	if (dictionary == NULL) {
		fprintf(stderr, "Error: %s: Dictionary pointer is NULL.\n", __func__);
		return 0;
	}
	return 1;
}

static inline int _validate_key(const char *key) {
	if (key == NULL || *key == '\0') {
		fprintf(stderr, "Error: %s: Key is NULL or empty.\n", __func__);
		return 0;
	}
	return 1;
}

static inline void _validate_entry_construction(entry *node) {
	if (!node) {
		fprintf(stderr, "Error: %s: Failed to construct dictionary entry node, aborting.\n", __func__);
		return;
	}
}

dict* dict_construct(unsigned long buckets) {
	if (buckets == 0) {
		fprintf(stderr, "=== error: dict_construct(): buckets cannot be 0 ===\n");
		return NULL;
	}

	dict* dictionary = malloc(sizeof(struct dict));

	if (!dictionary) {
		fprintf(stderr, "=== error: dict_construct(): malloc failed ===\n");
		return NULL;
	}

	dictionary->entries = calloc(buckets, sizeof(entry *));
	dictionary->buckets = buckets;

	if (!dictionary->entries) {
		fprintf(stderr, "=== error: dict_construct(): malloc failed ===\n");
		free(dictionary);
		return NULL;
	}


	for (unsigned long int i = 0; i < buckets; i++) {
		dictionary->entries[i] = NULL;
	}
	return dictionary;
}

void dict_destruct(dict* dictionary) {
	if (!_validate_dict_ptr(dictionary))
		return;
	if (dictionary->entries) {
		for (unsigned long i = 0; i < dictionary->buckets; i++) {
			entry *node = dictionary->entries[i];
			while(node) {
				entry *next = node->next;
				entry_destruct(node);
				node = next;
			}
		}

	}
	free(dictionary->entries);
	free(dictionary);
}

void* dict_search(dict* dictionary, const char* key) {
	if (!_validate_dict_ptr(dictionary) || !_validate_key(key))
		return NULL;

	unsigned int index = hash(dictionary, key);
	entry *node = dictionary->entries[index];

	while (node != NULL) {
		if (strcmp(node->key, key) == 0) {
			return node->value;
		}
		node = node->next;
	}
	fprintf(stderr, "Error: %s: Key '%s' not found in dictionary.\n", __func__, key);
	return NULL;
}

void dict_insert(dict* dictionary, const char* key, void* value, const td *type) {
	if (!_validate_dict_ptr(dictionary) || !_validate_key(key)) {
		return;
	}
	if (value == NULL) {
		fprintf(stderr, "Error: %s: Value pointer is NULL for key '%s'.\n", __func__, key);
		return;
	}

	unsigned int index = hash(dictionary, key);
	entry *node = dictionary->entries[index];
	/* insert at head of bucket chain */
	while(node != NULL) {
		if (strcmp(node->key,  key) == 0) {
			fprintf(stderr, "[dict:insert] duplicate key\n");
			return;
		}
		node = node->next;
	}

	entry *node_to_insert = entry_construct(key, value, type);
	_validate_entry_construction(node_to_insert);

	node_to_insert->next = dictionary->entries[index];
	dictionary->entries[index] = node_to_insert;
}

void dict_remove(dict* dictionary, const char* key) {
	if (!_validate_dict_ptr(dictionary) || !_validate_key(key))
		return;

	unsigned int index = hash(dictionary, key);
	entry *node = dictionary->entries[index];
	entry *prev = NULL;

	while (node != NULL) {
		if (strcmp(node->key, key) == 0) {
			if (prev == NULL) {
				dictionary->entries[index] = node->next;
			} else {
				prev->next = node->next;
			}
			entry_destruct(node);
			return;
		}
		prev = node;
		node = node->next;
	}

	fprintf(stderr, "Error: %s: Key '%s' not found in dictionary for removal.\n", __func__, key);
}

unsigned int hash(dict* dictionary, const char *key) {
	if (!_validate_key(key))
		return 0;

	unsigned long h = 5381;
	unsigned char c;
	while ((c = (unsigned char)*key++)) {
		h = ((h << 5) + h) + c; 
	}
	return (unsigned int)(h % dictionary->buckets);
} /* dict.c */
