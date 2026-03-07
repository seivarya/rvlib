/* <slist.c>: singly linked list methods */

#include <rvlib/slist.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static inline lib_status _validate_slist_ptr(slist *list) {
	if (list == NULL) {
		fprintf(stderr,
	  "[slist:validate_slist_ptr] Singly linked list pointer is NULL.\n");
		return LIB_PTR_INVALID;
	}
	return LIB_OK;
}

static inline lib_status _validate_sindex(slist *list, size_t index) {
	lib_status status = _validate_slist_ptr(list);
	if (status != LIB_OK)
		return status;

	if (index >= list->length) {
		fprintf(stderr,
	  "[slist:validate_sindex] Index %zu out of bounds for list length "
	  "%zu.\n",
	  index, list->length);
		return LIB_INDEX_ERR;
	}
	return LIB_OK;
}

static slist_node *_slist_iterate(slist *list, size_t index) {
	if (_validate_sindex(list, index) != LIB_OK)
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

lib_status slist_destruct(slist *list) {
	lib_status status = _validate_slist_ptr(list);
	if (status != LIB_OK)
		return status;

	/* destroy all nodes */
	slist_node *current = list->head;
	while (current != NULL) {
		slist_node *next = current->next;
		slist_node_destruct(current);
		current = next;
	}

	free(list);
	return LIB_OK;
}

lib_status slist_insert(slist *list, size_t index, void *data, const td *type) {
	lib_status status = _validate_slist_ptr(list);
	if (status != LIB_OK)
		return status;

	if (index > list->length) {
		fprintf(stderr,
	  "[slist:insert] Index %zu out of bounds for list length "
	  "%zu.\n",
	  index, list->length);
		return LIB_INDEX_ERR;
	}

	slist_node *new_node = slist_node_construct(data, type);
	if (!new_node) {
		return LIB_ERR_MALLOC;
	}

	/* insert at head */
	if (index == 0) {
		new_node->next = list->head;
		list->head = new_node;
	}
	/* insert at middle or end */
	else {
		slist_node *previous = _slist_iterate(list, index - 1);
		if (!previous) {
			slist_node_destruct(new_node);
			return LIB_INDEX_ERR;
		}
		new_node->next = previous->next;
		previous->next = new_node;
	}

	list->length++;
	return LIB_OK;
}

lib_status slist_remove(slist *list, size_t index) {
	lib_status status = _validate_sindex(list, index);
	if (status != LIB_OK)
		return status;

	slist_node *target;

	/* remove head */
	if (index == 0) {
		target = list->head;
		list->head = (list->length < 2) ? NULL : target->next;
	}
	/* remove middle/end */
	else {
		slist_node *previous = _slist_iterate(list, index - 1);
		if (!previous || !previous->next)
			return LIB_INDEX_ERR;
		target = previous->next;
		previous->next = target->next;
	}

	slist_node_destruct(target);
	list->length--;
	return LIB_OK;
}

slist_node *slist_fetch_node(slist *list, size_t index) {
	lib_status status = _validate_sindex(list, index);
	if (status != LIB_OK)
		return NULL;

	slist_node *node = _slist_iterate(list, index);
	if (!node) {
		fprintf(stderr, "[slist:fetch_node] node not found at index %zu.\n", index);
		return NULL;
	}
	return node;
}

lib_status slist_print(slist *list) {
	lib_status status = _validate_slist_ptr(list);
	if (status != LIB_OK)
		return status;

	if (list->length == 0) {
		fprintf(stderr,
	  "[slist:print] Singly linked list is empty, cannot print.\n");
		return LIB_LENGTH_ERR;
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
			return LIB_TD_FUNC_MISSING;
		}
		current = current->next;
	}
	return LIB_OK;
} /* <slist.c> */
