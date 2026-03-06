/* <slist.c>: singly linked list methods */

#include <rvlib/slist.h>
#include <stdio.h>
#include <stdlib.h>

/* info: private methods */

static slist_node *_slist_iterate(slist *list, size_t index) {

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

resv slist_destruct(slist *list) {
  if (!list)
    return RV_ERR_PTR_INVALID;

  /* destroy all nodes */
  slist_node *current = list->head;
  while (current != NULL) {
    slist_node *next = current->next;
    slist_node_destruct(current);
    current = next;
  }

  free(list);
  return RV_OK;
}

resv slist_insert(slist *list, size_t index, void *data, const td *type) {
  if (!list)
    return RV_ERR_PTR_INVALID;

  if (index > list->length) {
    return RV_ERR_OOB;
  }

  slist_node *new_node = slist_node_construct(data, type);
  if (!new_node)
    return RV_ERR_NOMEM;

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
  return RV_OK;
}

resv slist_remove(slist *list, size_t index) {
  if (!list)
    return RV_ERR_PTR_INVALID;
  if (index >= list->length)
    return RV_ERR_OOB;

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
  return RV_OK;
}

void *slist_fetch_node(slist *list, size_t index) {
  if (!list || index >= list->length)
    return NULL;

  slist_node *node = _slist_iterate(list, index);
  if (!node) {
    fprintf(stderr, "[slist:fetch_node] Node not found at index %zu.\n", index);
  }
  return node;
}

resv slist_print(slist *list) {
  if (!list)
    return RV_ERR_PTR_INVALID;

  if (list->length == 0) {
    return RV_ERR_EMPTY;
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
  return RV_OK;
} /* <slist.c> */
