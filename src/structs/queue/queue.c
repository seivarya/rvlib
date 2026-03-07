/* <queue.c>: queue methods */

#include <rvlib/queue.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline lib_status _validate_queue_ptr(queue *q) {
	if (q == NULL) {
		fprintf(stderr, "[queue:validate_queue_ptr] Queue pointer is NULL.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_queue_node_construction(queue *q,
							   queue_node *node) {
	if (!node) {
		queue_destruct(q);
		return LIB_ERR_MALLOC;
	}
	return LIB_OK;
}

static inline lib_status _validate_qindex(queue *q, size_t index) {
	if (q == NULL) {
		fprintf(stderr, "[queue:validate_qindex] Queue pointer is NULL for index "
	  "validation.\n");
		return LIB_PTR_INVALID;
	}
	if (index >= q->length) {
		fprintf(stderr,
	  "[queue:validate_qindex] Index %zu out of bounds for queue length "
	  "%zu.\n",
	  index, q->length);
		return LIB_INDEX_ERR;
	}
	return LIB_OK;
}

/* info: public methods */

queue *queue_construct(void) {
	queue *q = malloc(sizeof(queue));
	if (q) {
		q->head = NULL;
		q->tail = NULL;
		q->length = 0;
		return q;
	}

	fprintf(stderr, "[queue:construct] malloc failed\n");
	return NULL;
}

lib_status queue_destruct(queue *q) {
	if (_validate_queue_ptr(q) != LIB_OK)
		return LIB_PTR_INVALID;

	/* destroy all nodes */
	queue_node *current = q->head;
	while (current != NULL) {
		queue_node *next = current->next;
		queue_node_destruct(current);
		current = next;
	}

	free(q);
	return LIB_OK;
}

lib_status enqueue(queue *q, void *data, const td *type) {
	if (_validate_queue_ptr(q) != LIB_OK)
		return LIB_PTR_INVALID;

	queue_node *new_node = queue_node_construct(data, type);
	lib_status c_status = _validate_queue_node_construction(q, new_node);
	if (c_status != LIB_OK)
		return c_status;

	/* attach node to tail */
	if (q->length == 0) {
		q->head = new_node;
		q->tail = new_node;
	} else {
		q->tail->next = new_node;
		q->tail = new_node;
	}

	q->length++;
	return LIB_OK;
}

lib_status dequeue(queue *q) {
	if (_validate_queue_ptr(q) != LIB_OK)
		return LIB_PTR_INVALID;
	if (q->length == 0) {
		fprintf(stderr,
	  "[queue:dequeue] Attempted to dequeue from an empty queue.\n");
		return LIB_PTR_INVALID;
	}

	queue_node *target = q->head;

	if (q->length == 1) {
		q->head = NULL;
		q->tail = NULL;
	} else {
		q->head = target->next;
	}

	queue_node_destruct(target);
	q->length--;
	return LIB_OK;
}

int is_qempty(queue *q) {
	if (_validate_queue_ptr(q) != LIB_OK)
		return 1;

	return (q->length == 0);
}
// TODO: changes needed here! // #1
void *get_front(queue *q) {
	if (_validate_queue_ptr(q) != LIB_OK)
		return NULL;
	if (q->length == 0) {
		fprintf(stderr,
	  "[queue:get_front] Attempted to get front from an empty queue.\n");
		return NULL;
	}

	return q->head->data;
}

void *get_rear(queue *q) {
	if (_validate_queue_ptr(q) != LIB_OK)
		return NULL;
	if (q->length == 0) {
		fprintf(stderr,
	  "[queue:get_rear] Attempted to get rear from an empty queue.\n");
		return NULL;
	}

	return q->tail->data;
} /* <queue.c> */
