/* <deque.c>: deque methods */

#include <rvlib/deque.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline int _validate_deque_ptr(deque *dq) {
	if (dq == NULL) {
		fprintf(stderr, "[deque:validate_ptr] NULL deque pointer passed.\n");
		return 0;
	}
	return 1;
}

static inline void _validate_deque_node_construction(deque *dq,
						     deque_node *node) {
	if (!node) {
		deque_destruct(dq);
		exit(3);
	}
}

static inline int _validate_dqindex(deque *dq, size_t index) {
	if (dq == NULL) {
		fprintf(stderr, "[deque:validate_sindex] deque pointer is "
	  "NULL for index validation.\n");
		return 0;
	}
	if (index >= dq->length) {
		fprintf(stderr,
	  "[sdq:validate_sindex] Index %zu out of bounds for deque length "
	  "%zu.\n",
	  index, dq->length);
		return 0;
	}
	return 1;
}

deque *deque_construct(void) {
	deque *dq = malloc(sizeof(deque));
	if (dq) {
		dq->head = NULL;
		dq->tail = NULL;
		dq->length = 0;
		return dq;
	}

	fprintf(stderr, "[deque:construct] malloc failed");
	return NULL;
}

void deque_destruct(deque *dq) {
	if (!_validate_deque_ptr(dq))
		return;

	deque_node *current = dq->head;
	while (current != NULL) {
		deque_node *next = current->next;
		deque_node_destruct(current);
		current = next;
	}
	free(dq);
}

void push_front(deque *dq, void *data, const td *type) {
	if (!_validate_deque_ptr(dq))
		return;

	deque_node *node_to_insert = deque_node_construct(data, type);
	_validate_deque_node_construction(dq, node_to_insert);

	if (dq->length == 0) {
		dq->head = node_to_insert;
		dq->tail = node_to_insert;
	} else {
		node_to_insert->next = dq->head;
		dq->head->prev = node_to_insert;
		dq->head = node_to_insert;
	}
	dq->length++;
}

void push_back(deque *dq, void *data, const td *type) {
	if (!_validate_deque_ptr(dq))
		return;

	deque_node *node_to_insert = deque_node_construct(data, type);
	_validate_deque_node_construction(dq, node_to_insert);

	if (dq->length == 0) {
		dq->head = node_to_insert;
		dq->tail = node_to_insert;
	} else {
		node_to_insert->prev = dq->tail;
		dq->tail->next = node_to_insert;
		dq->tail = node_to_insert;
	}
	dq->length++;
}

void pop_front(deque *dq) {
	if (!_validate_deque_ptr(dq))
		return;

	if (dq->length == 0) {
		fprintf(stderr, "[deque:pop_front] Cannot pop from an empty deque.\n");
		return;
	}

	deque_node *node_to_remove = dq->head;
	dq->head = dq->head->next;

	if (dq->head) {
		dq->head->prev = NULL;
	} else {
		dq->tail = NULL;
	}

	deque_node_destruct(node_to_remove);
	dq->length--;
}

void pop_back(deque *dq) {
	if (!_validate_deque_ptr(dq))
		return;

	if (dq->length == 0) {
		fprintf(stderr, "[deque:pop_back] Cannot pop from an empty deque.\n");
		return;
	}

	deque_node *node_to_remove = dq->tail;
	dq->tail = dq->tail->prev;

	if (dq->tail) {
		dq->tail->next = NULL;
	} else {
		dq->head = NULL;
	}

	deque_node_destruct(node_to_remove);
	dq->length--;
}

void *peek_front(deque *dq) {
	if (!_validate_deque_ptr(dq) || dq->length == 0)
		return NULL;
	return dq->head->data;
}

void *peek_back(deque *dq) {
	if (!_validate_deque_ptr(dq) || dq->length == 0)
		return NULL;
	return dq->tail->data;
} /* <deque.c> */
