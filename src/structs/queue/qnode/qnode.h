/* qnode.h: queue node interface */

#ifndef QNODE_H
#define QNODE_H

#include <stdlib.h>
#include <rvlib/td.h>

typedef struct qnode qnode;

struct qnode {
	void *data;
	const td *type;
	qnode *next;
};

qnode* qnode_construct(void *data, const td *type);
void qnode_destruct(qnode *node);

#endif /* qnode_h */
