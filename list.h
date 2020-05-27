#ifndef EXTCLIB_LIST_H_
#define EXTCLIB_LIST_H_

#include <stddef.h>

#include "type.h"

typedef struct List List;

extern List *new_list(vtype_t type);
extern void free_list(List *list);

extern void push_list(List *list, void *value);
extern value_t pop_list(List *list);

extern value_t get_list(List *list, size_t index);
extern int8_t set_list(List *list, size_t index, void *value);
extern int8_t del_list(List *list, size_t index);
extern int32_t in_list(List *list, void *value);

extern int8_t cmp_list(List *x, List *y);
extern size_t size_list(List *list);
extern size_t sizeof_list(void);

extern void print_list(List *list);
extern void println_list(List *list);

#endif /* EXTCLIB_LIST_H_ */
