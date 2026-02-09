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

static void test_construct_destruct(void) {
	queue *q = queue_construct();
	CHECK(q != NULL);
	CHECK(q->head == NULL);
	CHECK(q->tail == NULL);
	CHECK(q->length == 0);
	CHECK(is_qempty(q) == 1);
	queue_destruct(q);
}

static void test_empty_operations(void) {
	queue *q = queue_construct();
	CHECK(is_qempty(q) == 1);
	CHECK(get_front(q) == NULL);
	CHECK(get_rear(q) == NULL);
	dequeue(q);
	queue_destruct(q);
}

static void test_enqueue_dequeue_single(void) {
	int x = 42;
	queue *q = queue_construct();
	enqueue(q, &x, &TD_INT);
	CHECK(q->length == 1);
	CHECK(is_qempty(q) == 0);
	CHECK(*(int*)get_front(q) == 42);
	CHECK(*(int*)get_rear(q) == 42);
	CHECK(q->head == q->tail);
	dequeue(q);
	CHECK(q->length == 0);
	CHECK(q->head == NULL);
	CHECK(q->tail == NULL);
	CHECK(is_qempty(q) == 1);
	CHECK(get_front(q) == NULL);
	CHECK(get_rear(q) == NULL);
	queue_destruct(q);
}

static void test_fifo_order(void) {
	int a=1,b=2,c=3;
	queue *q = queue_construct();

	enqueue(q,&a,&TD_INT);
	enqueue(q,&b,&TD_INT);
	enqueue(q,&c,&TD_INT);

	CHECK(q->length == 3);
	CHECK(*(int*)get_front(q) == 1);
	CHECK(*(int*)get_rear(q) == 3);

	CHECK(*(int*)get_front(q) == 1);
	dequeue(q);
	CHECK(*(int*)get_front(q) == 2);
	CHECK(*(int*)get_rear(q) == 3);

	dequeue(q);
	CHECK(*(int*)get_front(q) == 3);
	CHECK(*(int*)get_rear(q) == 3);

	dequeue(q);
	CHECK(q->length == 0);
	CHECK(is_qempty(q) == 1);
	CHECK(get_front(q) == NULL);
	CHECK(get_rear(q) == NULL);

	queue_destruct(q);
}

static void test_get_front_rear_do_not_remove(void) {
	int a=10,b=20;
	queue *q = queue_construct();
	enqueue(q,&a,&TD_INT);
	enqueue(q,&b,&TD_INT);

	CHECK(*(int*)get_front(q) == 10);
	CHECK(*(int*)get_rear(q) == 20);
	CHECK(*(int*)get_front(q) == 10);
	CHECK(q->length == 2);

	dequeue(q);
	CHECK(*(int*)get_front(q) == 20);
	CHECK(*(int*)get_rear(q) == 20);

	queue_destruct(q);
}

static void test_enqueue_after_dequeue_all(void) {
	int a=1,b=2;
	queue *q = queue_construct();
	enqueue(q,&a,&TD_INT);
	dequeue(q);
	enqueue(q,&b,&TD_INT);
	CHECK(q->length == 1);
	CHECK(*(int*)get_front(q) == 2);
	CHECK(*(int*)get_rear(q) == 2);
	queue_destruct(q);
}

static void test_multiple_enqueue_dequeue(void) {
	queue *q = queue_construct();
	int vals[20];
	for (int i=0;i<20;i++) vals[i] = i;

	for (int i=0;i<20;i++) enqueue(q,&vals[i],&TD_INT);
	CHECK(q->length == 20);
	CHECK(*(int*)get_front(q) == 0);
	CHECK(*(int*)get_rear(q) == 19);

	for (int i=0;i<20;i++) {
		CHECK(*(int*)get_front(q) == i);
		dequeue(q);
	}

	CHECK(q->length == 0);
	CHECK(is_qempty(q) == 1);

	queue_destruct(q);
}

static void test_head_tail_consistency(void) {
	int a=1,b=2,c=3;
	queue *q = queue_construct();

	enqueue(q,&a,&TD_INT);
	CHECK(q->head == q->tail);

	enqueue(q,&b,&TD_INT);
	CHECK(q->head != q->tail);
	CHECK(q->tail->next == NULL);

	enqueue(q,&c,&TD_INT);
	CHECK(q->tail->next == NULL);

	dequeue(q);
	dequeue(q);

	CHECK(q->head == q->tail);
	CHECK(*(int*)get_front(q) == 3);

	queue_destruct(q);
}

static void test_destruct_null(void) {
	queue_destruct(NULL);
}

static void test_is_qempty_null(void) {
	CHECK(is_qempty(NULL) == 1);
}

static void test_get_front_null(void) {
	CHECK(get_front(NULL) == NULL);
}

static void test_get_rear_null(void) {
	CHECK(get_rear(NULL) == NULL);
}

int main(void) {
	test_construct_destruct();
	test_empty_operations();
	test_enqueue_dequeue_single();
	test_fifo_order();
	test_get_front_rear_do_not_remove();
	test_enqueue_after_dequeue_all();
	test_multiple_enqueue_dequeue();
	test_head_tail_consistency();
	test_destruct_null();
	test_is_qempty_null();
	test_get_front_null();
	test_get_rear_null();

	if (failures > 0) {
		fprintf(stderr, "qtest: %d failure(s)\n", failures);
		return 1;
	}

	printf("qtest: all tests passed\n");
	return 0;
}
