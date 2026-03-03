/* slist_node.h: singly linked list node interface */

#ifndef SLIST_NODE_H
#define SLIST_NODE_H

#include <rvlib/td.h>

typedef struct slist_node slist_node;

struct slist_node {
  void *data;
  const td *type;
  slist_node *next;
};

slist_node *slist_node_construct(void *data, const td *type);
void slist_node_destruct(slist_node *node);

#endif /* slist_node_h */
