#include <rvlib/deque.h>
#include <rvlib/rvlib.h>
#include <stdio.h>
#include <stdlib.h>


static int failures = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
      failures++;                                                              \
    }                                                                          \
  } while (0)

static void test_construct_destruct(void) {
  deque *dq = deque_construct();
  CHECK(dq != NULL);
  CHECK(dq->head == NULL);
  CHECK(dq->tail == NULL);
  CHECK(dq->length == 0);
  CHECK(is_dqempty(dq) == 1);
  deque_destruct(dq);
}

static void test_empty_operations(void) {
  deque *dq = deque_construct();
  CHECK(is_dqempty(dq) == 1);
  CHECK(peek_front(dq) == NULL);
  CHECK(peek_back(dq) == NULL);
  pop_front(dq);
  pop_back(dq);
  deque_destruct(dq);
}

static void test_push_pop_front_single(void) {
  int x = 42;
  deque *dq = deque_construct();
  push_front(dq, &x, &TD_INT);
  CHECK(dq->length == 1);
  CHECK(is_dqempty(dq) == 0);
  CHECK(*(int *)peek_front(dq) == 42);
  CHECK(*(int *)peek_back(dq) == 42);
  CHECK(dq->head == dq->tail);

  pop_front(dq);
  CHECK(dq->length == 0);
  CHECK(dq->head == NULL);
  CHECK(dq->tail == NULL);
  CHECK(is_dqempty(dq) == 1);
  CHECK(peek_front(dq) == NULL);
  CHECK(peek_back(dq) == NULL);
  deque_destruct(dq);
}

static void test_push_pop_back_single(void) {
  int x = 42;
  deque *dq = deque_construct();
  push_back(dq, &x, &TD_INT);
  CHECK(dq->length == 1);
  CHECK(*(int *)peek_front(dq) == 42);
  CHECK(*(int *)peek_back(dq) == 42);
  CHECK(dq->head == dq->tail);

  pop_back(dq);
  CHECK(dq->length == 0);
  CHECK(dq->head == NULL);
  CHECK(dq->tail == NULL);
  deque_destruct(dq);
}

static void test_push_front_back_mixed(void) {
  int a = 1, b = 2, c = 3;
  deque *dq = deque_construct();

  push_front(dq, &b, &TD_INT); // dq: [2]
  push_front(dq, &a, &TD_INT); // dq: [1, 2]
  push_back(dq, &c, &TD_INT);  // dq: [1, 2, 3]

  CHECK(dq->length == 3);
  CHECK(*(int *)peek_front(dq) == 1);
  CHECK(*(int *)peek_back(dq) == 3);

  pop_front(dq); // dq: [2, 3]
  CHECK(*(int *)peek_front(dq) == 2);
  CHECK(*(int *)peek_back(dq) == 3);

  pop_back(dq); // dq: [2]
  CHECK(*(int *)peek_front(dq) == 2);
  CHECK(*(int *)peek_back(dq) == 2);

  pop_front(dq); // dq: []
  CHECK(dq->length == 0);
  CHECK(is_dqempty(dq) == 1);

  deque_destruct(dq);
}

static void test_get_front_rear_do_not_remove(void) {
  int a = 10, b = 20;
  deque *dq = deque_construct();
  push_back(dq, &a, &TD_INT);
  push_back(dq, &b, &TD_INT);

  CHECK(*(int *)peek_front(dq) == 10);
  CHECK(*(int *)peek_back(dq) == 20);
  CHECK(*(int *)peek_front(dq) == 10);
  CHECK(dq->length == 2);

  pop_front(dq);
  CHECK(*(int *)peek_front(dq) == 20);
  CHECK(*(int *)peek_back(dq) == 20);

  deque_destruct(dq);
}

static void test_multiple_push_pop(void) {
  deque *dq = deque_construct();
  int vals[20];
  for (int i = 0; i < 20; i++)
    vals[i] = i;

  for (int i = 0; i < 20; i++)
    push_back(dq, &vals[i], &TD_INT);
  CHECK(dq->length == 20);
  CHECK(*(int *)peek_front(dq) == 0);
  CHECK(*(int *)peek_back(dq) == 19);

  for (int i = 0; i < 20; i++) {
    CHECK(*(int *)peek_front(dq) == i);
    pop_front(dq);
  }

  CHECK(dq->length == 0);
  CHECK(is_dqempty(dq) == 1);

  deque_destruct(dq);
}

static void test_destruct_null(void) {
  deque_destruct(NULL); // Should safely return
}

static void test_is_dqempty_null(void) {
  // Our inline methods are checking dq->length directly,
  // so passing NULL will segfault if not validated.
  // Currently is_dqempty doesn't validate in deque.h since it's inline,
  // so we shouldn't test passing NULL to it unless we add validation.
  // For now, we will hold off this test to mimic existing queue semantics.
}

int main(void) {
  test_construct_destruct();
  test_empty_operations();
  test_push_pop_front_single();
  test_push_pop_back_single();
  test_push_front_back_mixed();
  test_get_front_rear_do_not_remove();
  test_multiple_push_pop();
  test_destruct_null();
  test_is_dqempty_null();

  if (failures > 0) {
    fprintf(stderr, "dqtest: %d failure(s)\n", failures);
    return 1;
  }

  printf("dqtest: all tests passed\n");
  return 0;
}
