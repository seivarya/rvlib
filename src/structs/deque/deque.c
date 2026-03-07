/* <deque.c>: deque methods */

#include <rvlib/deque.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline lib_status _validate_deque_ptr(deque *dq) {
	if (dq == NULL) {
		fprintf(stderr, "[deque:validate_ptr] NULL deque pointer passed.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_deque_node_construction(deque *dq,
							   deque_node *node) {
	if (!node) {
		deque_destruct(dq);
		return LIB_ERR_MALLOC;
	}
	return LIB_OK;
}

static inline lib_status _validate_dqindex(deque *dq, size_t index) {
	if (dq == NULL) {
		fprintf(stderr, "[deque:validate_sindex] deque pointer is "
	  "NULL for index validation.\n");
		return LIB_PTR_INVALID;
	}
	if (index >= dq->length) {
		fprintf(stderr,
	  "[sdq:validate_sindex] Index %zu out of bounds for deque length "
	  "%zu.\n",
	  index, dq->length);
		return LIB_INDEX_ERR;
	}
	return LIB_OK;
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

lib_status deque_destruct(deque *dq) {
	if (_validate_deque_ptr(dq) != LIB_OK)
		return LIB_PTR_INVALID;

	deque_node *current = dq->head;
	while (current != NULL) {
		deque_node *next = current->next;
		deque_node_destruct(current);
		current = next;
	}
	free(dq);
	return LIB_OK;
}

lib_status push_front(deque *dq, void *data, const td *type) {
	if (_validate_deque_ptr(dq) != LIB_OK)
		return LIB_PTR_INVALID;

	deque_node *node_to_insert = deque_node_construct(data, type);
	lib_status c_status = _validate_deque_node_construction(dq, node_to_insert);
	if (c_status != LIB_OK)
		return c_status;

	if (dq->length == 0) {
		dq->head = node_to_insert;
		dq->tail = node_to_insert;
	} else {
		node_to_insert->next = dq->head;
		dq->head->prev = node_to_insert;
		dq->head = node_to_insert;
	}
	dq->length++;
	return LIB_OK;
}

lib_status push_back(deque *dq, void *data, const td *type) {
	if (_validate_deque_ptr(dq) != LIB_OK)
		return LIB_PTR_INVALID;

	deque_node *node_to_insert = deque_node_construct(data, type);
	lib_status c_status = _validate_deque_node_construction(dq, node_to_insert);
	if (c_status != LIB_OK)
		return c_status;

	if (dq->length == 0) {
		dq->head = node_to_insert;
		dq->tail = node_to_insert;
	} else {
		node_to_insert->prev = dq->tail;
		dq->tail->next = node_to_insert;
		dq->tail = node_to_insert;
	}
	dq->length++;
	return LIB_OK;
}

lib_status pop_front(deque *dq) {
	if (_validate_deque_ptr(dq) != LIB_OK)
		return LIB_PTR_INVALID;

	if (dq->length == 0) {
		fprintf(stderr, "[deque:pop_front] Cannot pop from an empty deque.\n");
		return LIB_PTR_INVALID;
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
	return LIB_OK;
}

lib_status pop_back(deque *dq) {
	if (_validate_deque_ptr(dq) != LIB_OK)
		return LIB_PTR_INVALID;

	if (dq->length == 0) {
		fprintf(stderr, "[deque:pop_back] Cannot pop from an empty deque.\n");
		return LIB_PTR_INVALID;
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
	return LIB_OK;
}

void *peek_front(deque *dq) {
	if (_validate_deque_ptr(dq) != LIB_OK || dq->length == 0)
		return NULL;
	return dq->head->data;
}

void *peek_back(deque *dq) {
	if (_validate_deque_ptr(dq) != LIB_OK || dq->length == 0)
		return NULL;
	return dq->tail->data;
} /* <deque.c> */
