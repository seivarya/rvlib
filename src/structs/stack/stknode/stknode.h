/* stknode.h: stack node interface */

#ifndef STKNODE_H
#define STKNODE_H

#include <stdlib.h>
#include <rvlib/td.h> 

typedef struct stknode stknode;

struct stknode {
	void *data;
	const td *type;
	stknode *next;
};

stknode* stknode_construct(void *data, const td *type);
void stknode_destruct(stknode *node);

#endif /* stknode_h */
