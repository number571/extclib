#ifndef EXTCLIB_DYNAMIC_H_
#define EXTCLIB_DYNAMIC_H_

#include "type.h"

typedef struct Dynamic Dynamic;

extern Dynamic *new_dynamic(char ctype, void *value);
extern void free_dynamic(Dynamic *dynamic);

extern value_t value_dynamic(Dynamic *dynamic);
extern vtype_t type_dynamic(Dynamic *dynamic);

extern size_t sizeof_dynamic(void);
extern int8_t cmp_dynamic(Dynamic *x, Dynamic *y);

extern void print_dynamic(Dynamic *dynamic);
extern void println_dynamic(Dynamic *dynamic);

#endif /* EXTCLIB_DYNAMIC_H_ */
