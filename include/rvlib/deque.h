/* deque.h: deque header file */

#ifndef DEQUE_H
#define DEQUE_H

#include "td.h"
#include "deque/dqnode/dqnode.h"

typedef struct deque {
	dqnode *head;
	dqnode *tail;
	size_t length;
} deque;

deque* deque_construct(void);
void deque_destruct(deque *dq);

void push_front(deque *dq, void *data, const td *type); 
void push_back(deque *dq, void *data, const td *type);

void pop_front(deque *dq);
void pop_back(deque *dq);

void is_empty(deque *dq);

void* peek_front(deque *dq);
void* peek_back(deque *dq);
 
#endif
