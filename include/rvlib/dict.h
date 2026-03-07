/* dict.h: dictionary header file  */

#ifndef DICT_H
#define DICT_H

#define HASH_SIZE 100
#include "dict/dict_entry/dict_entry.h"
#include <rvlib/err.h>


typedef struct dict {
	unsigned long buckets;
	dict_entry **entries; // each arr element will have dict_entry *
} dict;

dict *dict_construct(unsigned long buckets);
lib_status dict_destruct(dict *dictionary);

void *dict_search(dict *dictionary, const char *key);
lib_status dict_insert(dict *dictionary, const char *key, void *value,
		       const td *type);
lib_status dict_remove(dict *dictionary, const char *key);

unsigned int hash(dict *dictionary, const char *key);

#endif /* dict_h */
