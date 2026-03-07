/* <clist.c>: circular linked list methods */

#include <rvlib/clist.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline lib_status _validate_clist_ptr(clist *list) {
	if (list == NULL) {
		fprintf(stderr,
	  "[clist:validate_clist_ptr] Circular list pointer is NULL.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_clist_node_construction(clist *list,
							   clist_node *node) {
	if (!node) {
		clist_destruct(list);
		return LIB_ERR_MALLOC;
	}
	return LIB_OK;
}

static inline lib_status _validate_cindex(clist *list, size_t index) {
	lib_status status = _validate_clist_ptr(list);
	if (status != LIB_OK) {
		return status;
	}
	if (index >= list->length) {
		fprintf(stderr,
	  "[clist:validate_cindex] Index %zu out of bounds for list length "
	  "%zu.\n",
	  index, list->length);
		return LIB_INDEX_ERR;
	}
	return LIB_OK;
}

static clist_node *_clist_iterate(clist *list, size_t index) {
	if (_validate_cindex(list, index) != LIB_OK)
		return NULL;

	size_t mid_index = (list->length / 2);
	clist_node *cursor;

	/* iterate */
	if (index <= mid_index) {
		cursor = list->head;
		while (index != 0) {
			cursor = cursor->next;
			index--;
		}
	} else {
		cursor = list->tail;
		size_t steps = list->length - index - 1; // index conversion
		while (steps != 0) {
			cursor = cursor->previous;
			steps--;
		}
	}

	return cursor;
}

clist *clist_construct(void) {
	clist *list = malloc(sizeof(clist));
	if (list) {
		list->head = NULL;
		list->tail = NULL;
		list->length = 0;
		return list;
	}
	fprintf(stderr, "[clist:construct] malloc failed\n");
	return NULL;
}

lib_status clist_destruct(clist *list) {
	if (_validate_clist_ptr(list) != LIB_OK)
		return LIB_PTR_INVALID;

	if (list->length == 0) {
		free(list);
		return LIB_PTR_INVALID;
	}

	clist_node *current = list->head;
	clist_node *head = list->head;
	do {
		clist_node *next = current->next;
		clist_node_destruct(current);
		current = next;
	} while (current != head);
	free(list);
	return LIB_OK;
}

lib_status clist_insert(clist *list, size_t index, void *data, const td *type) {
	if (_validate_clist_ptr(list) != LIB_OK)
		return LIB_PTR_INVALID;

	if (index > list->length) {
		fprintf(stderr, "[clist:insert] Index %zu is out of bounds (length %zu).\n",
	  index, list->length);
		return LIB_PTR_INVALID;
	}

	clist_node *new_node = clist_node_construct(data, type);
	lib_status c_status = _validate_clist_node_construction(list, new_node);
	if (c_status != LIB_OK)
		return c_status;

	/* case 1: insert at head */
	if (index == 0) {
		if (list->length == 0) {
			list->head = new_node;
			list->tail = new_node;

			new_node->next = new_node;
			new_node->previous = new_node;
		} else {
			new_node->next = list->head;
			new_node->previous = list->tail;

			list->head->previous = new_node;
			list->tail->next = new_node;

			list->head = new_node;
		}
	}

	/* case 2: insert at tail (append) */
	else if (index == list->length) {
		list->tail->next = new_node;
		list->head->previous = new_node;

		new_node->previous = list->tail;
		new_node->next = list->head;

		list->tail = new_node;
	}

	/* case 3: insert at middle */
	else {
		clist_node *on_index_node = _clist_iterate(list, index);

		new_node->previous = on_index_node->previous;
		new_node->next = on_index_node;

		on_index_node->previous->next = new_node;
		on_index_node->previous = new_node;
	}

	list->length++;
	return LIB_OK;
}

lib_status clist_remove(clist *list, size_t index) {
	if (_validate_cindex(list, index) != LIB_OK)
		return LIB_INDEX_ERR;

	clist_node *target;

	/* case 1: removing the head */
	if (index == 0) {
		target = list->head;

		if (list->length < 2) {
			list->head = NULL;
			list->tail = NULL;
		} else {
			list->head = target->next;

			list->tail->next = target->next;
			list->head->previous = list->tail;
		}
	}

	/* case 2: removing the tail */
	else if (index == list->length - 1) {
		target = list->tail;

		list->tail->previous->next = list->head;
		list->head->previous = target->previous;

		list->tail = target->previous;
	}

	/* case 3: removing from middle */
	else {
		target = _clist_iterate(list, index);

		target->previous->next = target->next;
		target->next->previous = target->previous;
	}

	clist_node_destruct(target);
	list->length--;
	return LIB_OK;
}

void *clist_fetch_node(clist *list, size_t index) {
	if (_validate_cindex(list, index) != LIB_OK)
		return NULL;

	clist_node *node = _clist_iterate(list, index);
	if (!node) {
		fprintf(stderr, "[clist:fetch_node] Node not found at index %zu.\n", index);
	}
	return node;
}

lib_status clist_print(clist *list) {
	if (_validate_clist_ptr(list) != LIB_OK)
		return LIB_PTR_INVALID;

	if (list->length == 0) {
		fprintf(stderr, "[clist:print] Circular list is empty, cannot print.\n");
		return LIB_PTR_INVALID;
	}

	clist_node *current = list->head;
	do {
		const td *type = current->type;
		if (type && type->print) {
			type->print(current->data);
		} else {
			fprintf(stderr,
	   "[clist:print] No print function available for data type at node "
	   "%p.\n",
	   (void *)current);
		}
		current = current->next;
	} while (current != list->head);
	return LIB_OK;
} /* <clist.c> */
