/* queue.h: queue header files */

#ifndef QUEUE_H
#define QUEUE_H

#include "td.h"
#include "queue/queue_node/queue_node.h"

typedef struct queue {
	queue_node *head;
	queue_node *tail;
	size_t length;
} queue;
	
queue* queue_construct(void);
void queue_destruct(queue *q);

void enqueue(queue *q, void *data, const td *type);
void dequeue(queue *q);
int is_qempty(queue *q);

void* get_front(queue *q);
void* get_rear(queue *q);

#endif /* queue_h */
