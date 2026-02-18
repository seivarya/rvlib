/* cnode.h: circular linked list node interface */

#ifndef CNODE_H
#define CNODE_H

#include <rvlib/td.h>

typedef struct cnode cnode;

struct cnode {
	void *data;
	cnode *next;
	const td *type;
	cnode *previous;
};

cnode* cnode_construct(void *data, const td *type);
void cnode_destruct(cnode *node);

#endif /* cnode_h */
