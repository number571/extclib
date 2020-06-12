#ifndef EXTCLIB_ARRAY_H_
#define EXTCLIB_ARRAY_H_

#include <stddef.h>

#include "type.h"

typedef struct Array Array;

extern Array *new_array(size_t size, vtype_t type);
extern void free_array(Array *array);

// ARRAY
extern value_t get_array(Array *array, size_t index);
extern int8_t set_array(Array *array, size_t index, void *value);
extern int8_t del_array(Array *array, size_t index);
extern int32_t in_array(Array *array, void *value);

extern int8_t cmp_array(Array *x, Array *y);
extern size_t size_array(Array *array);
extern size_t sizeof_array(void);

extern void print_array(Array *array);
extern void println_array(Array *array);

// STACK
extern int8_t push_stack(Array *array, void *value);
extern value_t pop_stack(Array *array);

extern int8_t cmp_stack(Array *x, Array *y);

extern size_t size_stack(Array *array);
extern int8_t set_stack(Array *array, size_t top, size_t begin, size_t end);

extern void print_stack(Array *array);
extern void println_stack(Array *array);

#endif /* EXTCLIB_ARRAY_H_ */
