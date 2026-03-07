/* <dict.c>: dictionary methods */

#include <rvlib/dict.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline lib_status _validate_dict_ptr(dict *dictionary) {
	if (dictionary == NULL) {
		fprintf(stderr, "[dict:validate_dict_ptr] Dictionary pointer is NULL.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_key(const char *key) {
	if (key == NULL || *key == '\0') {
		fprintf(stderr, "[dict:validate_key] Key is NULL or empty.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_dict_entry_construction(dict_entry *node) {
	if (!node) {
		fprintf(stderr, "[dict:validate_dict_entry_construction] Failed to "
	  "construct dictionary dict_entry node, aborting.\n");
		return LIB_ERR_MALLOC;
	}
	return LIB_OK;
}

dict *dict_construct(unsigned long buckets) {
	if (buckets == 0) {
		fprintf(stderr, "[dict:construct] buckets cannot be 0\n");
		return NULL;
	}

	dict *dictionary = malloc(sizeof(struct dict));

	if (!dictionary) {
		fprintf(stderr, "[dict:construct] malloc failed\n");
		return NULL;
	}

	dictionary->entries = calloc(buckets, sizeof(dict_entry *));
	dictionary->buckets = buckets;

	if (!dictionary->entries) {
		fprintf(stderr, "[dict:construct] malloc failed\n");
		free(dictionary);
		return NULL;
	}

	for (unsigned long int i = 0; i < buckets; i++) {
		dictionary->entries[i] = NULL;
	}
	return dictionary;
}

lib_status dict_destruct(dict *dictionary) {
	if (_validate_dict_ptr(dictionary) != LIB_OK)
		return LIB_PTR_INVALID;
	if (dictionary->entries) {
		for (unsigned long i = 0; i < dictionary->buckets; i++) {
			dict_entry *node = dictionary->entries[i];
			while (node) {
				dict_entry *next = node->next;
				dict_entry_destruct(node);
				node = next;
			}
		}
	}
	free(dictionary->entries);
	free(dictionary);
	return LIB_OK;
}

void *dict_search(dict *dictionary, const char *key) {
	if (_validate_dict_ptr(dictionary) != LIB_OK || _validate_key(key) != LIB_OK)
		return NULL;

	unsigned int index = hash(dictionary, key);
	dict_entry *node = dictionary->entries[index];

	while (node != NULL) {
		if (strcmp(node->key, key) == 0) {
			return node->value;
		}
		node = node->next;
	}
	fprintf(stderr, "[dict:search] Key '%s' not found in dictionary.\n", key);
	return NULL;
}

lib_status dict_insert(dict *dictionary, const char *key, void *value,
		       const td *type) {
	if (_validate_dict_ptr(dictionary) != LIB_OK ||
		_validate_key(key) != LIB_OK) {
		return LIB_PTR_INVALID;
	}
	if (value == NULL) {
		fprintf(stderr, "[dict:insert] Value pointer is NULL for key '%s'.\n", key);
		return LIB_PTR_INVALID;
	}

	unsigned int index = hash(dictionary, key);
	dict_entry *node = dictionary->entries[index];
	/* insert at head of bucket chain */
	while (node != NULL) {
		if (strcmp(node->key, key) == 0) {
			fprintf(stderr, "[dict:insert] duplicate key\n");
			return LIB_PTR_INVALID;
		}
		node = node->next;
	}

	dict_entry *node_to_insert = dict_entry_construct(key, value, type);
	lib_status c_status = _validate_dict_entry_construction(node_to_insert);
	if (c_status != LIB_OK)
		return c_status;

	node_to_insert->next = dictionary->entries[index];
	dictionary->entries[index] = node_to_insert;
	return LIB_OK;
}

lib_status dict_remove(dict *dictionary, const char *key) {
	if (_validate_dict_ptr(dictionary) != LIB_OK || _validate_key(key) != LIB_OK)
		return LIB_PTR_INVALID;

	unsigned int index = hash(dictionary, key);
	dict_entry *node = dictionary->entries[index];
	dict_entry *prev = NULL;

	while (node != NULL) {
		if (strcmp(node->key, key) == 0) {
			if (prev == NULL) {
				dictionary->entries[index] = node->next;
			} else {
				prev->next = node->next;
			}
			dict_entry_destruct(node);
			return LIB_OK;
		}
		prev = node;
		node = node->next;
	}

	fprintf(stderr,
	 "[dict:remove] Key '%s' not found in dictionary for removal.\n", key);
	return LIB_INDEX_ERR;
}

unsigned int hash(dict *dictionary, const char *key) {
	if (_validate_key(key) != LIB_OK)
		return 0;

	unsigned long h = 5381;
	unsigned char c;
	while ((c = (unsigned char)*key++)) {
		h = ((h << 5) + h) + c;
	}
	return (unsigned int)(h % dictionary->buckets);
} /* <dict.c> */
