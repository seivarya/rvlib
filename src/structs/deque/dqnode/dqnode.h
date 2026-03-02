#ifndef DQNODE_H
#define DQNODE_H

#include <stdlib.h>
#include <rvlib/td.h>

typedef struct dqnode dqnode;

struct dqnode {
	void *data;
	const td *type;
	dqnode *next;
	dqnode *prev;
};

dqnode* dqnode_construct(void *data, const td *type);
void dqnode_destruct(dqnode *node);

#endif
