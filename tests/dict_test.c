#include <rvlib/dict.h>
#include <rvlib/rvlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
      failures++;                                                              \
    }                                                                          \
  } while (0)

static void test_construct_destruct(void) {
  dict *d = dict_construct(100);
  CHECK(d != NULL);
  CHECK(d->buckets == 100);
  CHECK(d->entries != NULL);
  dict_destruct(d);
}

static void test_insert_search(void) {
  dict *d = dict_construct(10);
  int val1 = 42;
  int val2 = 100;

  dict_insert(d, "key1", &val1, &TD_INT);
  dict_insert(d, "key2", &val2, &TD_INT);

  void *res1 = dict_search(d, "key1");
  void *res2 = dict_search(d, "key2");

  CHECK(res1 != NULL);
  CHECK(*(int *)res1 == 42);

  CHECK(res2 != NULL);
  CHECK(*(int *)res2 == 100);

  dict_destruct(d);
}

static void test_search_not_found(void) {
  dict *d = dict_construct(10);
  int val = 10;
  dict_insert(d, "found", &val, &TD_INT);

  void *res = dict_search(d, "not_found");
  CHECK(res == NULL);

  dict_destruct(d);
}

static void test_insert_update(void) {
  dict *d = dict_construct(10);
  int val1 = 10;
  int val2 = 20;

  dict_insert(d, "key", &val1, &TD_INT);
  void *res1 = dict_search(d, "key");
  CHECK(res1 != NULL && *(int *)res1 == 10);

  dict_insert(d, "key", &val2, &TD_INT); // Update value for same key
  void *res2 = dict_search(d, "key");
  CHECK(res2 != NULL && *(int *)res2 == 20);

  dict_destruct(d);
}

static void test_remove(void) {
  dict *d = dict_construct(10);
  int val = 42;

  dict_insert(d, "key_to_remove", &val, &TD_INT);
  CHECK(dict_search(d, "key_to_remove") != NULL);

  dict_remove(d, "key_to_remove");
  CHECK(dict_search(d, "key_to_remove") == NULL);

  dict_destruct(d);
}

static void test_remove_not_found(void) {
  dict *d = dict_construct(10);
  int val = 42;

  dict_insert(d, "existing", &val, &TD_INT);
  dict_remove(d, "non_existent"); // Should handle gracefully
  CHECK(dict_search(d, "existing") != NULL);

  dict_destruct(d);
}

static void test_large_insertions(void) {
  dict *d = dict_construct(50); // Small bucket size to force collisions
  char key[20];

  for (int i = 0; i < 200; i++) {
    snprintf(key, sizeof(key), "key_%d", i);
    dict_insert(d, key, &i, &TD_INT);
  }

  for (int i = 0; i < 200; i++) {
    snprintf(key, sizeof(key), "key_%d", i);
    void *res = dict_search(d, key);
    CHECK(res != NULL);
    CHECK(*(int *)res == i);
  }

  dict_destruct(d);
}

static void test_destruct_null(void) {
  dict_destruct(NULL); // Should safely return
}

int main(void) {
  test_construct_destruct();
  test_insert_search();
  test_search_not_found();
  test_insert_update();
  test_remove();
  test_remove_not_found();
  test_large_insertions();
  test_destruct_null();

  if (failures > 0) {
    fprintf(stderr, "dict_test: %d failure(s)\n", failures);
    return 1;
  }

  printf("dict_test: all tests passed\n");
  return 0;
}
