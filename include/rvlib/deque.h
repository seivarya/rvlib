/* deque.h: deque header files */

#ifndef DEQUE_H
#define DEQUE_H

#include "deque/deque_node/deque_node.h"
#include "td.h"
#include <rvlib/err.h>

typedef struct deque {
	deque_node *head;
	deque_node *tail;
	size_t length;
} deque;

deque *deque_construct(void);
lib_status deque_destruct(deque *dq);

lib_status push_front(deque *dq, void *data, const td *type);
lib_status push_back(deque *dq, void *data, const td *type);

lib_status pop_front(deque *dq);
lib_status pop_back(deque *dq);

static inline int is_dqempty(deque *dq) { return dq->length == 0; }

static inline size_t get_dqlength(deque *dq) { return dq->length; }

void *peek_front(deque *dq);
void *peek_back(deque *dq);

#endif /* deque_h */
