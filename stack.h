#pragma once

#include <stddef.h>

#include "type.h"

typedef struct Stack {
    vtype_t type;
    value_t *buffer;
    size_t pointer;
    size_t size;
} Stack;

extern Stack *new_stack(size_t size, vtype_t type);
extern void free_stack(Stack *stack);
extern void print_stack(Stack *stack);
extern int8_t push_stack(Stack *stack, void *value);
extern value_t pop_stack(Stack *stack);
