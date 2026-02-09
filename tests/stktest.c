#include <stdio.h>
#include <stdlib.h>
#include <rvlib/rvlib.h>

static int failures = 0;

#define CHECK(cond) do { \
	if (!(cond)) { \
		fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#cond); \
		failures++; \
	} \
} while (0)

#define INT_AT_TOP(stk) (*(int*)peek((stk)))

static void test_construct_destruct(void) {
	stack *stk = stack_construct();
	CHECK(stk != NULL);
	CHECK(stk->head == NULL);
	CHECK(stk->length == 0);
	CHECK(is_empty(stk) == 1);
	stack_destruct(stk);
}

static void test_empty_operations(void) {
	stack *stk = stack_construct();
	CHECK(is_empty(stk) == 1);
	CHECK(peek(stk) == NULL);
	pop(stk);
	stack_destruct(stk);
}

static void test_push_pop_single(void) {
	int x=42;
	stack *stk = stack_construct();
	push(stk,&x,&TD_INT);
	CHECK(stk->length == 1);
	CHECK(is_empty(stk) == 0);
	CHECK(INT_AT_TOP(stk) == 42);
	pop(stk);
	CHECK(stk->length == 0);
	CHECK(is_empty(stk) == 1);
	CHECK(peek(stk) == NULL);
	stack_destruct(stk);
}

static void test_lifo_order(void) {
	int a=1,b=2,c=3;
	stack *stk = stack_construct();
	push(stk,&a,&TD_INT);
	push(stk,&b,&TD_INT);
	push(stk,&c,&TD_INT);
	CHECK(stk->length == 3);

	CHECK(INT_AT_TOP(stk) == 3);
	pop(stk);
	CHECK(INT_AT_TOP(stk) == 2);
	pop(stk);
	CHECK(INT_AT_TOP(stk) == 1);
	pop(stk);

	CHECK(stk->length == 0);
	CHECK(is_empty(stk) == 1);
	CHECK(peek(stk) == NULL);

	stack_destruct(stk);
}

static void test_push_after_pop(void) {
	int a=10,b=20;
	stack *stk = stack_construct();
	push(stk,&a,&TD_INT);
	pop(stk);
	push(stk,&b,&TD_INT);
	CHECK(stk->length == 1);
	CHECK(INT_AT_TOP(stk) == 20);
	pop(stk);
	CHECK(is_empty(stk) == 1);
	stack_destruct(stk);
}

static void test_peek_does_not_remove(void) {
	int x=7;
	stack *stk = stack_construct();
	push(stk,&x,&TD_INT);
	CHECK(INT_AT_TOP(stk) == 7);
	CHECK(INT_AT_TOP(stk) == 7);
	CHECK(stk->length == 1);
	pop(stk);
	CHECK(is_empty(stk) == 1);
	stack_destruct(stk);
}

static void test_multiple_push_pop(void) {
	stack *stk = stack_construct();
	int vals[20];
	for (int i=0;i<20;i++) vals[i] = i;
	for (int i=0;i<20;i++) push(stk,&vals[i],&TD_INT);

	CHECK(stk->length == 20);

	for (int i=19;i>=0;i--) {
		CHECK(INT_AT_TOP(stk) == i);
		pop(stk);
	}

	CHECK(stk->length == 0);
	CHECK(is_empty(stk) == 1);
	stack_destruct(stk);
}

static void test_destruct_null(void) {
	stack_destruct(NULL);
}

static void test_is_empty_null(void) {
	CHECK(is_empty(NULL) == 1);
}

static void test_peek_null(void) {
	CHECK(peek(NULL) == NULL);
}

int main(void) {
	test_construct_destruct();
	test_empty_operations();
	test_push_pop_single();
	test_lifo_order();
	test_push_after_pop();
	test_peek_does_not_remove();
	test_multiple_push_pop();
	test_destruct_null();
	test_is_empty_null();
	test_peek_null();

	if (failures > 0) {
		fprintf(stderr,"stktest: %d failure(s)\n",failures);
		return 1;
	}

	printf("stktest: all tests passed\n");
	return 0;
}
