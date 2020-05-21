#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "stack.h"

extern Stack *new_stack(size_t size, vtype_t type) {
    switch(type) {
        case DECIMAL_ELEM: case REAL_ELEM: case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "type not supported");
            return NULL;
    }
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->buffer = (value_t*)malloc(size * sizeof(value_t));
    stack->pointer = 0;
    stack->size = size;
    stack->type = type;
    return stack;
};

extern void print_stack(Stack *stack) {
    printf("[ ");
    for (size_t i = 0; i < stack->pointer; ++i) {
        switch(stack->type) {
            case DECIMAL_ELEM:
                printf("%d ", stack->buffer[i].decimal);
            break;
            case REAL_ELEM:
                printf("%lf ", stack->buffer[i].real);
            break;
            case STRING_ELEM:
                printf("'%s' ", stack->buffer[i].string);
            break;
        }
    }
    printf("]\n");
}

extern int8_t push_stack(Stack *stack, void *value) {
    if (stack->pointer == stack->size) {
        fprintf(stderr, "%s\n", "stack overflow");
        return 1;
    }
    switch(stack->type) {
        case DECIMAL_ELEM:
            stack->buffer[stack->pointer++].decimal = (int64_t)value;
        break;
        case REAL_ELEM:
            stack->buffer[stack->pointer++].real = *(double*)value;
            free((double*)value);
        break;
        case STRING_ELEM:
            stack->buffer[stack->pointer++].string = (uint8_t*)value;
        break;
    }
    return 0;
}

extern value_t pop_stack(Stack *stack) {
    if (stack->pointer == 0) {
        fprintf(stderr, "%s\n", "stack overflow");
        value_t none;
        return none;
    }
    return stack->buffer[--stack->pointer];
}

extern void free_stack(Stack *stack) {
    free(stack->buffer);
    free(stack);
}
