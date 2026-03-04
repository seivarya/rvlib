/* deque.h: deque header files */

#ifndef DEQUE_H
#define DEQUE_H

#include "deque/deque_node/deque_node.h"
#include "td.h"


typedef struct deque {
	deque_node *head;
	deque_node *tail;
	size_t length;
} deque;

deque *deque_construct(void);
void deque_destruct(deque *dq);

void push_front(deque *dq, void *data, const td *type);
void push_back(deque *dq, void *data, const td *type);

void pop_front(deque *dq);
void pop_back(deque *dq);

static inline int is_dqempty(deque *dq) { return dq->length == 0; }

static inline size_t get_dqlength(deque *dq) { return dq->length; }

void *peek_front(deque *dq);
void *peek_back(deque *dq);

#endif /* deque_h */
