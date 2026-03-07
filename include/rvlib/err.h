#ifndef ERR_H
#define ERR_H

typedef enum {
	LIB_OK = 0,
	LIB_ERR_IO,
	LIB_PTR_INVALID,
	LIB_ERR_NOMEM,
	LIB_INDEX_ERR,
	LIB_LENGTH_ERR,
	LIB_ERR_MALLOC,
	LIB_TD_FUNC_MISSING
} lib_status;

#endif
