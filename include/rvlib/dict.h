/* dict.h: */
#ifndef DICT_H
#define DICT_H

#define HASH_SIZE 100
#include "dict/entry/entry.h"

typedef struct dict {
	entry *entries[HASH_SIZE];
} dict;

dict* dict_construct(void);
void* dict_search(dict* dict, const char* key);
void dict_insert(dict* dict, const char* key, const void* value, const td *type);
void dict_remove(dict*dict, const char* key);

unsigned int hash(const char *key);

#endif
