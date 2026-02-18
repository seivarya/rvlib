/* slist.c: singly linked list methods */

#include <stdlib.h>
#include <stdio.h>
#include <rvlib/td.h>
#include <rvlib/slist.h>

#include "snode/snode.h" 

/* info: private methods */

static inline int _validate_slist_ptr(slist *list) {
	if (list == NULL) {
		fprintf(stderr, "Error: %s: Singly linked list pointer is NULL.\n", __func__);
		return 0;
	}
	return 1;
}

static inline void _validate_snode_construction(slist *list, snode *node) {
	if (!node) {
		slist_destruct(list);
		exit(3);
	}
}

static inline int _validate_sindex(slist *list, size_t index) {
	if (list == NULL) {
		fprintf(stderr, "Error: %s: Singly linked list pointer is NULL for index validation.\n", __func__);
		return 0;
	}
	if (index >= list->length) {
		fprintf(stderr, "Error: %s: Index %zu out of bounds for list length %zu.\n", __func__, index, list->length);
		return 0;
	}
	return 1;
}

static snode* _slist_iterate(slist *list, size_t index) {
	if (!_validate_sindex(list, index)) return NULL;

	snode *cursor = list->head;
	for (size_t i = 0; i < index; i++)
		cursor = cursor->next;

	return cursor;
}

/* info: public methods */

slist* slist_construct(void) {
	slist *list = malloc(sizeof(slist));
	if (list) {
		list->head = NULL;
		list->length = 0;
		return list;
	}
	fprintf(stderr, "=== error: slist_construct(): malloc failed ===\n");
	return NULL;
}

void slist_destruct(slist *list) {
	if (!_validate_slist_ptr(list)) return;

	/* destroy all nodes */
	snode *current = list->head;
	while (current != NULL) {
		snode *next = current->next;
		snode_destruct(current);
		current = next;
	}

	free(list);
}

void slist_insert(slist *list, size_t index, void *data, const td *type) {
	if (!_validate_slist_ptr(list)) return;

	if (index > list->length) {
		fprintf(stderr, "Error: %s: Index %zu is out of bounds (length %zu).\n", __func__, index, list->length);
		return;
	}

	snode *new_node = snode_construct(data, type);
	_validate_snode_construction(list, new_node);

	/* insert at head */
	if (index == 0) {
		new_node->next = list->head;
		list->head = new_node;
	}

	/* insert at middle or end */
	else {
		snode *previous = _slist_iterate(list, index - 1);
		new_node->next = previous->next;
		previous->next = new_node;
	}

	list->length++;
}

void slist_remove(slist *list, size_t index) {
	if (!_validate_sindex(list, index))
		return;

	snode *target;

	/* remove head */
	if (index == 0) {
		target = list->head;
		list->head = (list->length < 2) ? NULL : target->next;
	}

	/* remove middle/end */
	else {
		snode *previous = _slist_iterate(list, index - 1);
		target = previous->next;
		previous->next = target->next;
	}

	snode_destruct(target);
	list->length--;
}

void* slist_fetch_node(slist *list, size_t index) {
	if (!_validate_sindex(list, index))
		return NULL;

	snode *node = _slist_iterate(list, index);
	if (!node) { fprintf(stderr, "Error: %s: Node not found at index %zu.\n", __func__, index); }
	return node;
}

void slist_print(slist *list) {
	if (!_validate_slist_ptr(list))
		return;

	if (list->length == 0) {
		fprintf(stderr, "Error: %s: Singly linked list is empty, cannot print.\n", __func__);
		return;
	}

	snode *current = list->head;
	while (current != NULL) {
		const td *type = current->type;
		if (type && type->print) {
			type->print(current->data);
		} else {
			fprintf(stderr, "Warning: %s: No print function available for data type at node %p.\n", __func__, (void*)current);
		}
		current = current->next;
	}
} /* slist_c */
