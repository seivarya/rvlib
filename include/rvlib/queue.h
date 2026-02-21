/* queue.h: queue header files */

#ifndef QUEUE_H
#define QUEUE_H

#include "td.h"
#include "queue/qnode/qnode.h"

typedef struct queue {
	qnode *head;
	qnode *tail;
	size_t length;
} queue;
	
queue* queue_construct(void);
void queue_destruct(struct queue *q);

void enqueue(queue *q, void *data, const td *type);
void dequeue(queue *q);
int is_qempty(queue *q);

void* get_front(queue *q);
void* get_rear(queue *q);

#endif /* queue_h */
