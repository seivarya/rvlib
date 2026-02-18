/* dnode.h: doubly linked list node interface */

#ifndef DNODE_H
#define DNODE_H

#include <stdlib.h>
#include <rvlib/td.h>

typedef struct dnode dnode;

struct dnode {
	void *data;
	dnode *next;
	const td *type;
	dnode *previous;
};

dnode* dnode_construct(void *data, const td *type);
void dnode_destruct(dnode *node);

#endif /* dnode_h */
