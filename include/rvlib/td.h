/* td.h: type descriptors */

#ifndef TD_H
#define TD_H

#define TD_MAGIC 0x6969

#include <stddef.h>
#include <stdint.h>

typedef struct td td;

struct td {
	uint32_t magic;
	size_t size;
	void (*print)(void *); 
	size_t (*hash)(void *key);
	int (*compare)(void *a, void *b); 
	void (*destruct)(void *data); // user's custom destructor for objects
	void * (*copy)(void *data); 

};

int td_validator(const td *t);

void print_uint(void *data);
void print_long(void *data);
void print_ulong(void *data);
void print_short(void *data);
void print_ushort(void *data);
void print_bool(void *data);
void print_size_t(void *data);
void print_str(void *data);
void print_char(void *data);
void print_int(void *data);

size_t hash_int(void *data);
size_t hash_str(void *data);
size_t hash_uint(void *data);
size_t hash_long(void *data);
size_t hash_ulong(void *data);
size_t hash_short(void *data);
size_t hash_ushort(void *data);
size_t hash_bool(void *data);
size_t hash_size_t(void *data);
size_t hash_char(void *data);

int compare_int(void *a, void *b);
int compare_str(void *a, void *b);
int compare_uint(void *a, void *b);
int compare_long(void *a, void *b);
int compare_ulong(void *a, void *b);
int compare_short(void *a, void *b);
int compare_ushort(void *a, void *b);
int compare_bool(void *a, void *b);
int compare_size_t(void *a, void *b);
int compare_char(void *a, void *b);

extern const td TD_INT;
extern const td TD_STR;
extern const td TD_UINT;
extern const td TD_LONG;
extern const td TD_ULONG;
extern const td TD_SHORT;
extern const td TD_USHORT;
extern const td TD_BOOL;
extern const td TD_SIZE_T;
extern const td TD_CHAR;

#endif /* td_h */
