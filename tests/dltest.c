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

#define DLIST_NODE_AT(list,i) ((dlist_node*)dlist_fetch_node((list),(i)))
#define INT_AT(list,i) (*(int*)DLIST_NODE_AT((list),(i))->data)

static void test_construct_destruct(void) {
	dlist *list = dlist_construct();
	CHECK(list != NULL);
	CHECK(list->head == NULL);
	CHECK(list->tail == NULL);
	CHECK(list->length == 0);
	dlist_destruct(list);
}

static void test_empty_operations(void) {
	dlist *list = dlist_construct();
	CHECK(list != NULL);
	CHECK(dlist_fetch_node(list, 0) == NULL);
	dlist_remove(list, 0);
	dlist_destruct(list);
}

static void test_insert_at_head(void) {
	int a=10,b=20,c=30;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(INT_AT(list,0) == 10);

	dlist_insert(list,0,&b,&TD_INT);
	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 20);
	CHECK(INT_AT(list,1) == 10);

	dlist_insert(list,0,&c,&TD_INT);
	CHECK(list->length == 3);
	CHECK(INT_AT(list,0) == 30);
	CHECK(INT_AT(list,1) == 20);
	CHECK(INT_AT(list,2) == 10);

	dlist_destruct(list);
}

static void test_insert_at_tail(void) {
	int a=1,b=2,c=3;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_insert(list,2,&c,&TD_INT);

	CHECK(list->length == 3);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 2);
	CHECK(INT_AT(list,2) == 3);

	dlist_destruct(list);
}

static void test_insert_at_middle(void) {
	int a=1,b=2,c=3,x=99;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_insert(list,2,&c,&TD_INT);
	dlist_insert(list,1,&x,&TD_INT);

	CHECK(list->length == 4);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 99);
	CHECK(INT_AT(list,2) == 2);
	CHECK(INT_AT(list,3) == 3);

	dlist_destruct(list);
}

static void test_remove_head(void) {
	int a=1,b=2,c=3;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_insert(list,2,&c,&TD_INT);

	dlist_remove(list,0);
	CHECK(list->length == 2);
	CHECK(DLIST_NODE_AT(list,0)->previous == NULL);
	CHECK(INT_AT(list,0) == 2);
	CHECK(INT_AT(list,1) == 3);

	dlist_remove(list,0);
	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(INT_AT(list,0) == 3);

	dlist_remove(list,0);
	CHECK(list->length == 0);
	CHECK(list->head == NULL);
	CHECK(list->tail == NULL);

	dlist_destruct(list);
}

static void test_remove_tail(void) {
	int a=1,b=2,c=3;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_insert(list,2,&c,&TD_INT);

	dlist_remove(list,2);
	CHECK(list->length == 2);
	CHECK(DLIST_NODE_AT(list,1)->next == NULL);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 2);

	dlist_remove(list,1);
	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(INT_AT(list,0) == 1);

	dlist_destruct(list);
}

static void test_remove_middle(void) {
	int a=1,b=2,c=3;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_insert(list,2,&c,&TD_INT);

	dlist_remove(list,1);

	CHECK(list->length == 2);
	CHECK(INT_AT(list,0) == 1);
	CHECK(INT_AT(list,1) == 3);
	CHECK(list->head->next == list->tail);
	CHECK(list->tail->previous == list->head);

	dlist_destruct(list);
}

static void test_fetch_node_out_of_bounds(void) {
	int a=1;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);

	CHECK(dlist_fetch_node(list,1) == NULL);
	CHECK(dlist_fetch_node(list,99) == NULL);

	dlist_destruct(list);
}

static void test_single_element(void) {
	int x=42;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&x,&TD_INT);

	CHECK(list->length == 1);
	CHECK(list->head == list->tail);
	CHECK(list->head->previous == NULL);
	CHECK(list->head->next == NULL);
	CHECK(INT_AT(list,0) == 42);

	dlist_remove(list,0);

	CHECK(list->length == 0);

	dlist_destruct(list);
}

static void test_forward_backward_links(void) {
	int a=1,b=2,c=3;
	dlist *list = dlist_construct();

	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_insert(list,2,&c,&TD_INT);

	dlist_node *n0 = DLIST_NODE_AT(list,0);
	dlist_node *n1 = DLIST_NODE_AT(list,1);
	dlist_node *n2 = DLIST_NODE_AT(list,2);

	CHECK(n0->previous == NULL);
	CHECK(n0->next == n1);
	CHECK(n1->previous == n0);
	CHECK(n1->next == n2);
	CHECK(n2->previous == n1);
	CHECK(n2->next == NULL);
	CHECK(list->head == n0);
	CHECK(list->tail == n2);

	dlist_destruct(list);
}

static void test_print_empty(void) {
	dlist *list = dlist_construct();
	dlist_print(list);
	dlist_destruct(list);
}

static void test_print_non_empty(void) {
	int a=1,b=2;
	dlist *list = dlist_construct();
	dlist_insert(list,0,&a,&TD_INT);
	dlist_insert(list,1,&b,&TD_INT);
	dlist_print(list);
	dlist_destruct(list);
}

static void test_destruct_null(void) {
	dlist_destruct(NULL);
}

static void test_sequence(void) {
	dlist *list = dlist_construct();
	int vals[10]={0,1,2,3,4,5,6,7,8,9};

	for (size_t i=0;i<10;i++)
		dlist_insert(list,i,&vals[i],&TD_INT);

	CHECK(list->length == 10);

	for (size_t i=0;i<10;i++)
		CHECK(INT_AT(list,i) == (int)i);

	dlist_destruct(list);
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
	test_forward_backward_links();
	test_print_empty();
	test_print_non_empty();
	test_destruct_null();
	test_sequence();

	if (failures > 0) {
		fprintf(stderr, "dltest: %d failure(s)\n", failures);
		return 1;
	}

	printf("dltest: all tests passed\n");
	return 0;
}
