/* clist.c: circualr linked list methods */

#include <stdlib.h>
#include <stdio.h>
#include <rvlib/td.h>
#include <rvlib/clist.h>

#include "cnode/cnode.h" 

/* info: private methods */

static inline int _validate_clist_ptr(clist *list) {
	if (list == NULL) {
		fprintf(stderr, "Error: %s: Circular list pointer is NULL.\n", __func__);
		return 0;
	}
	return 1;
}

static inline void _validate_cnode_construction(clist *list, cnode *node) {
	if (!node) {
		clist_destruct(list);
		exit(3);
	}
}

static inline int _validate_cindex(clist *list, size_t index) {
	if (list == NULL) {
		fprintf(stderr, "Error: %s: Circular list pointer is NULL for index validation.\n", __func__);
		return 0;
	}
	if (index >= list->length) {
		fprintf(stderr, "Error: %s: Index %zu out of bounds for list length %zu.\n", __func__, index, list->length);
		return 0;
	}
	return 1;
}

static cnode* _clist_iterate(clist *list, size_t index) {
	if (!_validate_cindex(list, index)) 
		return NULL;

	size_t mid_index = (list->length / 2);
	cnode *cursor;

	/* iterate */
	if (index <= mid_index) {
		cursor = list->head;
		while(index != 0) {
			cursor = cursor->next;
			index--;
		}
	} else {
		cursor = list->tail;
		size_t steps = list->length - index - 1; // index conversion
		while(steps != 0) {
			cursor = cursor->previous;
			steps--;
		}
	}

	return cursor;
}

clist* clist_construct(void) {
	clist* list = malloc(sizeof(clist));
	if (list) {
		list->head = NULL;	
		list->tail = NULL;
		list->length = 0;
		return list;
	}
	fprintf(stderr, "=== error: clist_construct(): malloc failed ===\n");
	return NULL;
}

void clist_destruct(clist *list) {
	if (!_validate_clist_ptr(list)) 
		return;

	if (list->length == 0) {
		free(list);
		return;
	}

	cnode *current = list->head;
	cnode *head = list->head;
	do {
		cnode *next = current->next;
		cnode_destruct(current);
		current = next;
	} while (current != head);
	free(list);
}

void clist_insert(clist *list, size_t index, void *data, const td *type) {
	if (!_validate_clist_ptr(list))
		return;

	if (index > list->length) {
		fprintf(stderr, "Error: %s: Index %zu is out of bounds (length %zu).\n", __func__, index, list->length);
		return;
	}

	cnode *new_node = cnode_construct(data, type);
	_validate_cnode_construction(list, new_node);

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
		cnode *on_index_node = _clist_iterate(list, index);

		new_node->previous = on_index_node->previous;
		new_node->next = on_index_node;

		on_index_node->previous->next = new_node;
		on_index_node->previous = new_node;
	}

	list->length++;
}


void clist_remove(clist *list, size_t index) {
	if (!_validate_cindex(list, index))
		return;

	cnode *target;

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

	cnode_destruct(target);
	list->length--;
}

void* clist_fetch_node(clist *list, size_t index) {
	if (!_validate_cindex(list, index)) return NULL;

	cnode *node = _clist_iterate(list, index);
	if (!node) { fprintf(stderr, "Error: %s: Node not found at index %zu.\n", __func__, index); }
	return node;
}

void clist_print(clist *list) {
	if (!_validate_clist_ptr(list))
		return;

	if (list->length == 0) {
		fprintf(stderr, "Error: %s: Circular list is empty, cannot print.\n", __func__);
		return;
	}

	cnode *current = list->head;
	do {
		const td *type = current->type;
		if (type && type->print) {
			type->print(current->data);
		} else {
			fprintf(stderr, "Warning: %s: No print function available for data type at node %p.\n", __func__, (void*)current);
		}
		current = current->next;
	} while(current != list->head);
} /* clist_c */
