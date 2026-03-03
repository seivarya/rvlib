/* <slist.c>: singly linked list methods */

#include <rvlib/slist.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline int _validate_slist_ptr(slist *list) {
	if (list == NULL) {
		fprintf(stderr,
	  "[slist:validate_slist_ptr] Singly linked list pointer is NULL.\n");
		return 0;
	}
	return 1;
}

static inline void _validate_slist_node_construction(slist *list, slist_node *node) {
	if (!node) {
		slist_destruct(list);
		exit(3);
	}
}

static inline int _validate_sindex(slist *list, size_t index) {
	if (list == NULL) {
		fprintf(stderr, "[slist:validate_sindex] Singly linked list pointer is "
	  "NULL for index validation.\n");
		return 0;
	}
	if (index >= list->length) {
		fprintf(stderr,
	  "[slist:validate_sindex] Index %zu out of bounds for list length "
	  "%zu.\n",
	  index, list->length);
		return 0;
	}
	return 1;
}

static slist_node *_slist_iterate(slist *list, size_t index) {
	if (!_validate_sindex(list, index))
		return NULL;

	slist_node *cursor = list->head;
	for (size_t i = 0; i < index; i++)
		cursor = cursor->next;

	return cursor;
}

/* info: public methods */

slist *slist_construct(void) {
	slist *list = malloc(sizeof(slist));
	if (list) {
		list->head = NULL;
		list->length = 0;
		return list;
	}
	fprintf(stderr, "[slist:construct] malloc failed\n");
	return NULL;
}

void slist_destruct(slist *list) {
	if (!_validate_slist_ptr(list))
		return;

	/* destroy all nodes */
	slist_node *current = list->head;
	while (current != NULL) {
		slist_node *next = current->next;
		slist_node_destruct(current);
		current = next;
	}

	free(list);
}

void slist_insert(slist *list, size_t index, void *data, const td *type) {
	if (!_validate_slist_ptr(list))
		return;

	if (index > list->length) {
		fprintf(stderr, "[slist:insert] Index %zu is out of bounds (length %zu).\n",
	  index, list->length);
		return;
	}

	slist_node *new_node = slist_node_construct(data, type);
	_validate_slist_node_construction(list, new_node);

	/* insert at head */
	if (index == 0) {
		new_node->next = list->head;
		list->head = new_node;
	}

/* insert at middle or end */
	else {
		slist_node *previous = _slist_iterate(list, index - 1);
		new_node->next = previous->next;
		previous->next = new_node;
	}

	list->length++;
}

void slist_remove(slist *list, size_t index) {
	if (!_validate_sindex(list, index))
		return;

	slist_node *target;

	/* remove head */
	if (index == 0) {
		target = list->head;
		list->head = (list->length < 2) ? NULL : target->next;
	}

	/* remove middle/end */
	else {
		slist_node *previous = _slist_iterate(list, index - 1);
		target = previous->next;
		previous->next = target->next;
	}

	slist_node_destruct(target);
	list->length--;
}

void *slist_fetch_node(slist *list, size_t index) {
	if (!_validate_sindex(list, index))
		return NULL;

	slist_node *node = _slist_iterate(list, index);
	if (!node) {
		fprintf(stderr, "[slist:fetch_node] Node not found at index %zu.\n", index);
	}
	return node;
}

void slist_print(slist *list) {
	if (!_validate_slist_ptr(list))
		return;

	if (list->length == 0) {
		fprintf(stderr,
	  "[slist:print] Singly linked list is empty, cannot print.\n");
		return;
	}

	slist_node *current = list->head;
	while (current != NULL) {
		const td *type = current->type;
		if (type && type->print) {
			type->print(current->data);
		} else {
			fprintf(stderr,
	   "[slist:print] No print function available for data type at node "
	   "%p.\n",
	   (void *)current);
		}
		current = current->next;
	}
}
/* <slist.c> */
