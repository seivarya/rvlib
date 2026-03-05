#ifndef ERR_H
#define ERR_H

typedef enum {
	RV_OK = 0,
	RV_ERR_IO,
	RV_ERR_PTR_INVALID,
	RV_ERR_NOMEM
} resv;

#endif
