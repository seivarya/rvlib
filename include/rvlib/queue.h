/* queue.h: queue header files */

#ifndef QUEUE_H
#define QUEUE_H

#include "queue/queue_node/queue_node.h"
#include "td.h"
#include <rvlib/err.h>


typedef struct queue {
	queue_node *head;
	queue_node *tail;
	size_t length;
} queue;

queue *queue_construct(void);
lib_status queue_destruct(queue *q);

lib_status enqueue(queue *q, void *data, const td *type);
lib_status dequeue(queue *q);
int is_qempty(queue *q);

void *get_front(queue *q);
void *get_rear(queue *q);

#endif /* queue_h */
