/* <dlist.c>: doubly linked list methods */

#include <rvlib/dlist.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline int _validate_dlist_ptr(dlist *list) {
	if (list == NULL) {
		fprintf(stderr,
	  "[dlist:validate_dlist_ptr] Doubly linked list pointer is NULL.\n");
		return 0;
	}
	return 1;
}

static inline void _validate_dlist_node_construction(dlist *list, dlist_node *node) {
	if (!node) {
		dlist_destruct(list);
		exit(3);
	}
}

static inline int _validate_dindex(dlist *list, size_t index) {
	if (list == NULL) {
		fprintf(stderr, "[dlist:validate_dindex] Doubly linked list pointer is "
	  "NULL for index validation.\n");
		return 0;
	}
	if (index >= list->length) {
		fprintf(stderr,
	  "[dlist:validate_dindex] Index %zu out of bounds for list length "
	  "%zu.\n",
	  index, list->length);
		return 0;
	}
	return 1;
}

static dlist_node *_dlist_iterate(dlist *list, size_t index) {
	if (!_validate_dindex(list, index))
		return NULL;

	size_t mid_index = list->length / 2;
	dlist_node *cursor;

	/* iterate: optimized */
	if (index <= mid_index) {
		cursor = list->head;
		while (index != 0) {
			cursor = cursor->next;
			index--;
		}
	} else {
		cursor = list->tail;
		size_t steps = list->length - index - 1;
		while (steps != 0) {
			cursor = cursor->previous;
			steps--;
		}
	}
	return cursor;
}

/* info: public methods */

dlist *dlist_construct(void) {
	dlist *list = malloc(sizeof(dlist));
	if (list) {
		list->head = NULL;
		list->tail = NULL;
		list->length = 0;
		return list;
	}
	fprintf(stderr, "[dlist:construct] malloc failed\n");
	return NULL;
}

void dlist_destruct(dlist *list) {
	if (!_validate_dlist_ptr(list))
		return;

	dlist_node *current = list->head;
	while (current != NULL) {
		dlist_node *next = current->next;
		dlist_node_destruct(current);
		current = next;
	}

	free(list);
}

void dlist_insert(dlist *list, size_t index, void *data, const td *type) {
	if (!_validate_dlist_ptr(list))
		return;

	if (index > list->length) {
		fprintf(stderr, "[dlist:insert] Index %zu is out of bounds (length %zu).\n",
	  index, list->length);
		return;
	}

	dlist_node *new_node = dlist_node_construct(data, type);
	_validate_dlist_node_construction(list, new_node);

	/* case 1: insert at head */
	if (index == 0) {
		if (list->length == 0) {
			list->head = list->tail = new_node;
		} else {
			new_node->next = list->head;
			new_node->previous = NULL;
			list->head->previous = new_node;
			list->head = new_node;
		}
	}

	/* case 2: insert at tail */
	else if (index == list->length) {
		new_node->previous = list->tail;
		list->tail->next = new_node;
		list->tail = new_node; /* new_node->next: NULL: default */
	}

	/* case 3: insert in the middle */
	else {
		dlist_node *on_index_node = _dlist_iterate(list, index);
		new_node->previous = on_index_node->previous;
		new_node->next = on_index_node;

		on_index_node->previous->next = new_node;
		on_index_node->previous = new_node;
	}

	list->length++;
}

void dlist_remove(dlist *list, size_t index) {
	if (!_validate_dindex(list, index))
		return;

	dlist_node *target;

	/* case 1: removing the head */
	if (index == 0) {
		target = list->head;

		if (list->length < 2) {
			list->head = NULL;
			list->tail = NULL;
		} else {
			list->head = target->next;
			list->head->previous = NULL;
		}
	}

	/* case 2: removing the tail */
	else if (index == list->length - 1) {
		target = list->tail;

		list->tail->previous->next = NULL;
		list->tail = list->tail->previous;
	}

	/* case 3: removing from middle */
	else {
		target = _dlist_iterate(list, index);

		target->previous->next = target->next;
		target->next->previous = target->previous;
	}

	dlist_node_destruct(target);
	list->length--;
}

void *dlist_fetch_node(dlist *list, size_t index) {
	if (!_validate_dindex(list, index))
		return NULL;

	dlist_node *node = _dlist_iterate(list, index);
	if (!node) {
		fprintf(stderr, "[dlist:fetch_node] Node not found at index %zu.\n", index);
	}
	return node;
}

void dlist_print(dlist *list) {
	if (!_validate_dlist_ptr(list))
		return;

	if (list->length == 0) {
		fprintf(stderr,
	  "[dlist:print] Doubly linked list is empty, cannot print.\n");
		return;
	}

	dlist_node *current = list->head;
	while (current != NULL) {
		const td *type = current->type;
		if (type && type->print) {
			type->print(current->data);
		} else {
			fprintf(stderr,
	   "[dlist:print] No print function available for data type at node "
	   "%p.\n",
	   (void *)current);
		}
		current = current->next;
	}
}
/* <dlist.c> */
