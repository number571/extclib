#ifndef EXTCLIB_STACK_H_
#define EXTCLIB_STACK_H_

#include <stddef.h>

#include "type.h"

typedef struct Stack Stack;

extern Stack *new_stack(size_t size, vtype_t type);
extern void free_stack(Stack *stack);

extern int8_t push_stack(Stack *stack, void *value);
extern value_t pop_stack(Stack *stack);

extern int8_t cmp_stack(Stack *x, Stack *y);
extern size_t size_stack(Stack *stack);
extern size_t sizeof_stack(void);

extern void print_stack(Stack *stack);
extern void println_stack(Stack *stack);

#endif /* EXTCLIB_STACK_H_ */
