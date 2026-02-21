<div align="center">
  <h3>rvlib</h3>
  a minimal c library providing reusable data structures and utilities.
  <br>
  <h4>data structures & api</h4>
</div>

##### circular linked list ([clist.h](include/rvlib/clist.h))
- `clist* clist_construct(void)`
- `void clist_destruct(clist *list)`
- `void clist_insert(clist *list, size_t index, void *data, const td *type)`
- `void clist_remove(clist *list, size_t index)`
- `void clist_print(clist *list)`
- `void* clist_fetch_node(clist *list, size_t index)`

##### dictionary ([dict.h](include/rvlib/dict.h))
- `dict* dict_construct(unsigned long buckets)`
- `void dict_destruct(dict* dictionary)`
- `void* dict_search(dict* dictionary, const char* key)`
- `void dict_insert(dict* dictionary, const char* key, void* value, const td *type)`
- `void dict_remove(dict* dictionary, const char* key)`
- `unsigned int hash(dict* dictionary, const char *key)`

##### doubly linked list ([dlist.h](include/rvlib/dlist.h))
- `dlist* dlist_construct(void)`
- `void dlist_destruct(dlist *list)`
- `void dlist_insert(dlist *list, size_t index, void *data, const td *type)`
- `void dlist_remove(dlist *list, size_t index)`
- `void dlist_print(dlist *list)`
- `void* dlist_fetch_node(dlist *list, size_t index)`

##### queue ([queue.h](include/rvlib/queue.h))
- `queue* queue_construct(void)`
- `void queue_destruct(queue *q)`
- `void enqueue(queue *q, void *data, const td *type)`
- `void dequeue(queue *q)`
- `int is_qempty(queue *q)`
- `void* get_front(queue *q)`
- `void* get_rear(queue *q)`

##### singly linked list ([slist.h](include/rvlib/slist.h))
- `slist* slist_construct(void)`
- `void slist_destruct(slist *list)`
- `void slist_insert(slist *list, size_t index, void *data, const td *type)`
- `void slist_remove(slist *list, size_t index)`
- `void slist_print(slist *list)`
- `void* slist_fetch_node(slist *list, size_t index)`

##### stack ([stack.h](include/rvlib/stack.h))
- `stack* stack_construct(void)`
- `void stack_destruct(stack *stk)`
- `void push(stack *stk, void *data, const td *type)`
- `void pop(stack *stk)`
- `int is_empty(stack *stk)`
- `void* peek(stack *stk)`

<br>

<div align="center">
  <h4>build instructions</h4>
</div>

To compile everything and build the test suites, you can run the provided `makefile`:

```sh
make all
