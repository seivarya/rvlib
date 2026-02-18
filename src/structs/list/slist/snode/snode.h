/* snode.h: singly linked list node interface */

#ifndef SNODE_H
#define SNODE_H

#include <rvlib/td.h>

typedef struct snode snode;

struct snode {
	void *data;
	const td *type;
	snode *next;
};

snode* snode_construct(void *data, const td *type);
void snode_destruct(snode *node);

#endif /* snode_h */
