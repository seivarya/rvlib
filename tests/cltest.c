#include <stdio.h>
#include <stdlib.h>
#include <rvlib/rvlib.h>

static int failures = 0;

#define CHECK(cond) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
		failures++; \
	} \
} while (0)

#define CNODE_AT(list,i) ((cnode*)clist_fetch_node((list),(i)))
#define INT_AT(list,i) (*(int*)CNODE_AT((list),(i))->data)

static void test_construct_destruct(void) {
	clist *list = clist_construct();
	CHECK(list != NULL);
	CHECK(list->head == NULL);
	CHECK(list->tail == NULL);
	CHECK(list->length == 0);
	clist_destruct(list);
}

static void test_empty_operations(void) {
	clist *list = clist_construct();
	CHECK(clist_fetch_node(list,0) == NULL);
	clist_remove(list,0);
	clist_destruct(list);
}

static void test_insert_at_head(void) {
	int a=10,b=20,c=30;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(list->head->next == list->head);
	CHECK(list->head->previous == list->head);

	clist_insert(list,0,&b,&TD_INT);
	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 20);
	CHECK(INT_AT(list,1) == 10);

	clist_insert(list,0,&c,&TD_INT);
	CHECK(list->length == 3);
	CHECK(INT_AT(list,0) == 30);
	CHECK(INT_AT(list,1) == 20);
	CHECK(INT_AT(list,2) == 10);

	clist_destruct(list);
}

static void test_insert_at_tail(void) {
	int a=1,b=2,c=3;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);
	clist_insert(list,2,&c,&TD_INT);

	CHECK(list->length == 3);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 2);
	CHECK(INT_AT(list,2) == 3);
	CHECK(list->tail->next == list->head);
	CHECK(list->head->previous == list->tail);

	clist_destruct(list);
}

static void test_insert_at_middle(void) {
	int a=1,b=2,c=3,x=99;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);
	clist_insert(list,2,&c,&TD_INT);
	clist_insert(list,1,&x,&TD_INT);

	CHECK(list->length == 4);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 99);
	CHECK(INT_AT(list,2) == 2);
	CHECK(INT_AT(list,3) == 3);

	clist_destruct(list);
}

static void test_remove_head(void) {
	int a=1,b=2,c=3;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);
	clist_insert(list,2,&c,&TD_INT);

	clist_remove(list,0);
	CHECK(list->length == 2);
	CHECK(list->tail->next == list->head);
	CHECK(list->head->previous == list->tail);
	CHECK(INT_AT(list,0) == 2);
	CHECK(INT_AT(list,1) == 3);

	clist_remove(list,0);
	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(list->head->next == list->head);
	CHECK(list->head->previous == list->head);

	clist_remove(list,0);
	CHECK(list->length == 0);
	CHECK(list->head == NULL);
	CHECK(list->tail == NULL);

	clist_destruct(list);
}

static void test_remove_tail(void) {
	int a=1,b=2,c=3;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);
	clist_insert(list,2,&c,&TD_INT);

	clist_remove(list,2);
	CHECK(list->length == 2);
	CHECK(list->tail->next == list->head);
	CHECK(list->head->previous == list->tail);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 2);

	clist_remove(list,1);
	CHECK(list->length == 1);
	CHECK(list->head == list->tail);

	clist_destruct(list);
}

static void test_remove_middle(void) {
	int a=1,b=2,c=3;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);
	clist_insert(list,2,&c,&TD_INT);

	clist_remove(list,1);

	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 3);
	CHECK(list->head->next->next == list->head);
	CHECK(list->tail->previous->previous == list->tail);

	clist_destruct(list);
}

static void test_fetch_node_out_of_bounds(void) {
	int a=1;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);

	CHECK(clist_fetch_node(list,1) == NULL);
	CHECK(clist_fetch_node(list,99) == NULL);

	clist_destruct(list);
}

static void test_single_element(void) {
	int x=42;
	clist *list = clist_construct();

	clist_insert(list,0,&x,&TD_INT);

	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(list->head->next == list->head);
	CHECK(list->head->previous == list->head);
	CHECK(INT_AT(list,0) == 42);

	clist_remove(list,0);
	CHECK(list->length == 0);

	clist_destruct(list);
}

static void test_circular_links(void) {
	int a=1,b=2,c=3;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);
	clist_insert(list,2,&c,&TD_INT);

	cnode *n0 = CNODE_AT(list,0);
	cnode *n1 = CNODE_AT(list,1);
	cnode *n2 = CNODE_AT(list,2);

	CHECK(n0->previous == n2);
	CHECK(n0->next == n1);
	CHECK(n1->previous == n0);
	CHECK(n1->next == n2);
	CHECK(n2->previous == n1);
	CHECK(n2->next == n0);
	CHECK(list->head == n0);
	CHECK(list->tail == n2);

	clist_destruct(list);
}

static void test_print_empty(void) {
	clist *list = clist_construct();
	clist_print(list);
	clist_destruct(list);
}

static void test_print_non_empty(void) {
	int a=1,b=2;
	clist *list = clist_construct();

	clist_insert(list,0,&a,&TD_INT);
	clist_insert(list,1,&b,&TD_INT);

	clist_print(list);

	clist_destruct(list);
}

static void test_destruct_null(void) {
	clist_destruct(NULL);
}

static void test_sequence(void) {
	clist *list = clist_construct();
	int vals[10]={0,1,2,3,4,5,6,7,8,9};

	for (size_t i=0;i<10;i++)
		clist_insert(list,i,&vals[i],&TD_INT);

	CHECK(list->length == 10);

	for (size_t i=0;i<10;i++)
		CHECK(INT_AT(list,i) == (int)i);

	CHECK(list->tail->next == list->head);
	CHECK(list->head->previous == list->tail);

	clist_destruct(list);
}

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
	test_circular_links();
	test_print_empty();
	test_print_non_empty();
	test_destruct_null();
	test_sequence();

	if (failures > 0) {
		fprintf(stderr, "cltest: %d failure(s)\n", failures);
		return 1;
	}

	printf("cltest: all tests passed\n");
	return 0;
}
