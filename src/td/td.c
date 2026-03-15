/* <td.c>: type descriptor methods */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rvlib/td.h"

int td_validator(const td *t) {
	if (t == NULL) {
		fprintf(stderr,
	  "[td:validator] null type descriptor provided for validation.\n");
		return 0;
	}
	return 1;
}

void print_uint(void *data) { printf("%u\n", *(unsigned int *)data); }
void print_long(void *data) { printf("%ld\n", *(long *)data); }
void print_ulong(void *data) { printf("%lu\n", *(unsigned long *)data); }
void print_short(void *data) { printf("%hd\n", *(short *)data); }
void print_ushort(void *data) { printf("%hu\n", *(unsigned short *)data); }
void print_bool(void *data) {
	printf("%s\n", (*(int *)data) ? "true" : "false");
}
void print_size_t(void *data) { printf("%zu\n", *(size_t *)data); }
void print_str(void *data) { printf("%s\n", (char *)data); }
void print_char(void *data) { printf("%c\n", *(char *)data); }
void print_int(void *data) { printf("%d\n", *(int *)data); }

size_t hash_int(void *data) { return (size_t)(*(int *)data); }

/* simple djb2 string hash */
size_t hash_str(void *data) {
	unsigned char *s = (unsigned char *)data;
	int h = 5381;
	int c;
	while ((c = *s++))
		h = ((h << 5) + h) + c;
	return (size_t)h;
}

size_t hash_uint(void *data) { return (size_t)(*(unsigned int *)data); }
size_t hash_long(void *data) { return (size_t)(*(long *)data); }
size_t hash_ulong(void *data) { return (size_t)(*(unsigned long *)data); }
size_t hash_short(void *data) { return (size_t)(*(short *)data); }
size_t hash_ushort(void *data) { return (size_t)(*(unsigned short *)data); }
size_t hash_bool(void *data) { return (size_t)(*(int *)data); }
size_t hash_size_t(void *data) { return *(size_t *)data; }
size_t hash_char(void *data) { return (size_t)(*(char *)data); }

int compare_int(void *a, void *b) {
	int x = *(int *)a, y = *(int *)b;
	return x - y;
}

int compare_str(void *a, void *b) { return strcmp((char *)a, (char *)b); }

int compare_uint(void *a, void *b) {
	unsigned int x = *(unsigned int *)a, y = *(unsigned int *)b;
	return (x > y) - (x < y);
}

int compare_long(void *a, void *b) {
	long x = *(long *)a, y = *(long *)b;
	return (x > y) - (x < y);
}

int compare_ulong(void *a, void *b) {
	unsigned long x = *(unsigned long *)a, y = *(unsigned long *)b;
	return (x > y) - (x < y);
}

int compare_short(void *a, void *b) {
	short x = *(short *)a, y = *(short *)b;
	return (x > y) - (x < y);
}

int compare_ushort(void *a, void *b) {
	unsigned short x = *(unsigned short *)a, y = *(unsigned short *)b;
	return (x > y) - (x < y);
}

int compare_bool(void *a, void *b) {
	int x = *(int *)a, y = *(int *)b;
	return x - y;
}

int compare_size_t(void *a, void *b) {
	size_t x = *(size_t *)a, y = *(size_t *)b;
	return (x > y) - (x < y);
}

int compare_char(void *a, void *b) {
	char x = *(char *)a, y = *(char *)b;
	return (x > y) - (x < y);
}

const td TD_STR = {
	.size = sizeof(char *),
	.print = print_str,
	.hash = hash_str,
	.compare = compare_str,
	.copy = NULL,
	.destruct = NULL};

const td TD_INT = {
	.size = sizeof(int),
	.print = print_int,
	.hash = hash_int,
	.compare = compare_int,
	.copy = NULL,
	.destruct = NULL};

const td TD_UINT = {
	.size = sizeof(unsigned int),
	.print = print_uint,
	.hash = hash_uint,
	.compare = compare_uint,
	.copy = NULL,
	.destruct = NULL};

const td TD_LONG = {
	.size = sizeof(long),
	.print = print_long,
	.hash = hash_long,
	.compare = compare_long,
	.copy = NULL,
	.destruct = NULL};

const td TD_ULONG = {
	.size = sizeof(unsigned long),
	.print = print_ulong,
	.hash = hash_ulong,
	.compare = compare_ulong,
	.copy = NULL,
	.destruct = NULL};

const td TD_SHORT = {
	.size = sizeof(short),
	.print = print_short,
	.hash = hash_short,
	.compare = compare_short,
	.copy = NULL,
	.destruct = NULL};

const td TD_USHORT = {
	.size = sizeof(unsigned short),
	.print = print_ushort,
	.hash = hash_ushort,
	.compare = compare_ushort,
	.copy = NULL,
	.destruct = NULL};

const td TD_BOOL = {
	.size = sizeof(int),
	.print = print_bool,
	.hash = hash_bool,
	.compare = compare_bool,
	.copy = NULL,
	.destruct = NULL};

const td TD_SIZE_T = {
	.size = sizeof(size_t),
	.print = print_size_t,
	.hash = hash_size_t,
	.compare = compare_size_t,
	.copy = NULL,
	.destruct = NULL};

const td TD_CHAR = {
	.size = sizeof(char),
	.print = print_char,
	.hash = hash_char,
	.compare = compare_char,
	.copy = NULL,
	.destruct = NULL};
/* <td.c> */
