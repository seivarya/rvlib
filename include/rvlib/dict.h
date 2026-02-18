/* dict.h: */
#ifndef DICT_H
#define DICT_H

#define HASH_SIZE 100
#include "dict/entry/entry.h"

typedef struct dict {
	unsigned long buckets;
	entry **entries; // each arr element will have entry *
} dict;

dict* dict_construct(unsigned long buckets);
void dict_destruct(dict* dictionary);

void* dict_search(dict* dictionary, const char* key);
void dict_insert(dict* dictionary, const char* key, void* value, const td *type);
void dict_remove(dict*dictionary, const char* key);

unsigned int hash(dict* dictionary, const char *key);

#endif
