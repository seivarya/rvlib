#ifndef DICT_ENTRY_H
#define DICT_ENTRY_H

#include <rvlib/td.h>

typedef struct dict_entry dict_entry;

struct dict_entry {
	char* key;
	void* value;
	const td *type;
	dict_entry *next;
};

dict_entry* dict_entry_construct(const char* key, void* value, const td *type);
void dict_entry_destruct(dict_entry *dict_entry);

#endif /* dict_entry_h */
