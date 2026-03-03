#include <stdio.h>
#include <string.h>
#include <rvlib/slist.h>
#include <rvlib/td.h>

/* ---------------------- TEST HARNESS ---------------------- */

static int failures = 0;
#define CHECK(cond) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
		failures++; \
	} \
} while (0)

#define NODE_AT(list, i) ((slist_node*)slist_fetch_node((list), (i)))
#define INT_AT(list, i) (*(int*)NODE_AT((list), (i))->data)

/* ---------------------- TESTS ---------------------- */

static void test_construct_destruct(void) {
	slist *list = slist_construct();
	CHECK(list != NULL);
	CHECK(list->head == NULL);
	CHECK(list->length == 0);
	slist_destruct(list);
}

static void test_empty_operations(void) {
	slist *list = slist_construct();
	CHECK(slist_fetch_node(list, 0) == NULL);
	slist_remove(list, 0); /* should not crash */
	slist_destruct(list);
}

static void test_insert_at_head(void) {
	int a=10,b=20,c=30;
	slist *list = slist_construct();

	slist_insert(list, 0, &a, &TD_INT);
	CHECK(list->length == 1);
	CHECK(INT_AT(list,0) == 10);

	slist_insert(list, 0, &b, &TD_INT);
	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 20);
	CHECK(INT_AT(list,1) == 10);

	slist_insert(list, 0, &c, &TD_INT);
	CHECK(list->length == 3);
	CHECK(INT_AT(list,0) == 30);
	CHECK(INT_AT(list,1) == 20);
	CHECK(INT_AT(list,2) == 10);

	slist_destruct(list);
}

static void test_insert_at_tail(void) {
	int a=1,b=2,c=3;
	slist *list = slist_construct();

	slist_insert(list,0,&a,&TD_INT);
	slist_insert(list,1,&b,&TD_INT);
	slist_insert(list,2,&c,&TD_INT);

	CHECK(list->length == 3);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 2);
	CHECK(INT_AT(list,2) == 3);

	slist_destruct(list);
}

static void test_insert_at_middle(void) {
	int a=1,b=2,c=3,x=99;
	slist *list = slist_construct();

	slist_insert(list,0,&a,&TD_INT);
	slist_insert(list,1,&b,&TD_INT);
	slist_insert(list,2,&c,&TD_INT);

	slist_insert(list,1,&x,&TD_INT);

	CHECK(list->length == 4);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 99);
	CHECK(INT_AT(list,2) == 2);
	CHECK(INT_AT(list,3) == 3);

	slist_destruct(list);
}

static void test_remove_head(void) {
	int a=1,b=2,c=3;
	slist *list = slist_construct();

	slist_insert(list,0,&a,&TD_INT);
	slist_insert(list,1,&b,&TD_INT);
	slist_insert(list,2,&c,&TD_INT);

	slist_remove(list,0);
	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 2);
	CHECK(INT_AT(list,1) == 3);

	slist_remove(list,0);
	CHECK(list->length == 1);
	CHECK(INT_AT(list,0) == 3);

	slist_remove(list,0);
	CHECK(list->length == 0);
	CHECK(list->head == NULL);

	slist_destruct(list);
}

static void test_remove_tail(void) {
	int a=1,b=2,c=3;
	slist *list = slist_construct();

	slist_insert(list,0,&a,&TD_INT);
	slist_insert(list,1,&b,&TD_INT);
	slist_insert(list,2,&c,&TD_INT);

	slist_remove(list,2);
	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 2);

	slist_remove(list,1);
	CHECK(list->length == 1);
	CHECK(INT_AT(list,0) == 1);

	slist_destruct(list);
}

static void test_remove_middle(void) {
	int a=1,b=2,c=3;
	slist *list = slist_construct();

	slist_insert(list,0,&a,&TD_INT);
	slist_insert(list,1,&b,&TD_INT);
	slist_insert(list,2,&c,&TD_INT);

	slist_remove(list,1);
	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 3);

	slist_destruct(list);
}

static void test_fetch_node_out_of_bounds(void) {
	int a=1;
	slist *list = slist_construct();
	slist_insert(list,0,&a,&TD_INT);

	CHECK(slist_fetch_node(list,1) == NULL);
	CHECK(slist_fetch_node(list,999) == NULL);

	slist_destruct(list);
}

static void test_single_element(void) {
	int x=42;
	slist *list = slist_construct();

	slist_insert(list,0,&x,&TD_INT);
	CHECK(list->length == 1);
	CHECK(INT_AT(list,0) == 42);

	slist_remove(list,0);
	CHECK(list->length == 0);

	slist_destruct(list);
}

static void test_print_empty(void) {
	slist *list = slist_construct();
	slist_print(list); /* should not crash */
	slist_destruct(list);
}

static void test_print_non_empty(void) {
	int a=1,b=2;
	slist *list = slist_construct();

	slist_insert(list,0,&a,&TD_INT);
	slist_insert(list,1,&b,&TD_INT);

	slist_print(list);

	slist_destruct(list);
}

static void test_destruct_null(void) {
	slist_destruct(NULL); /* must not crash */
}

static void test_insert_append_sequence(void) {
	slist *list = slist_construct();
	int vals[10]={0,1,2,3,4,5,6,7,8,9};

	for (size_t i=0;i<10;i++)
		slist_insert(list, i, &vals[i], &TD_INT);

	CHECK(list->length == 10);

	for (int i=0;i<10;i++)
		CHECK(INT_AT(list,(size_t)i) == i);

	slist_destruct(list);
}

/* -------- STRUCT TYPE TEST -------- */

struct test_data {
	int elone;
	char *eltwo; /* static string OK */
};

static void print_struct(void *data) {
	struct test_data *d = data;
	printf("{ elone=%d, eltwo=%s }\n", d->elone, d->eltwo);
}

static void test_struct_copy_and_print(void) {
	slist *list = slist_construct();

	struct test_data data = { 32, "static-string" };

	td TD_STRUCT = {
		.magic = TD_MAGIC,
		.size = sizeof(struct test_data),
		.print = print_struct,
		.hash = NULL,
		.compare = NULL,
		.destruct = NULL,  /* shallow struct, no cleanup needed */
		.copy = NULL       /* memcpy fallback is fine */
	};

	slist_insert(list, 0, &data, &TD_STRUCT);

	struct test_data *retrieved = (struct test_data *)NODE_AT(list, 0)->data;

	CHECK(retrieved->elone == 32);
	CHECK(strcmp(retrieved->eltwo, "static-string") == 0);

	slist_print(list); /* should print struct */

	slist_destruct(list);
}

/* ---------------------- MAIN ---------------------- */

int main(void) {
	test_construct_destruct();
	test_empty_operations();
	test_insert_at_head();
	test_insert_at_tail();
	test_insert_at_middle();
	test_remove_head();
	test_remove_tail();
	test_remove_middle();
	test_fetch_node_out_of_bounds();
	test_single_element();
	test_print_empty();
	test_print_non_empty();
	test_destruct_null();
	test_insert_append_sequence();
	test_struct_copy_and_print();

	if (failures > 0) {
		fprintf(stderr, "sltest: %d failure(s)\n", failures);
		return 1;
	}

	printf("sltest: all tests passed\n");
	return 0;
}
