#ifndef ENTRY_H
#define ENTRY_H

#include <rvlib/td.h>

typedef struct entry entry;

struct entry {
	char* key;
	void* value;
	const td *type;
	entry *next;
};

entry* entry_construct(const char* key, void* value, const td *type);
void entry_destruct(entry *entry);

#endif /* entry_h */
