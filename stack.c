#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "type.h"
#include "tree.h"
#include "list.h"
#include "hashtab.h"
#include "stack.h"
#include "bigint.h"

typedef struct Stack {
    vtype_t type;
    value_t *buffer;
    size_t pointer;
    size_t size;
} Stack;

static void _free_stack(Stack *stack);

extern Stack *new_stack(size_t size, vtype_t type) {
    switch(type) {
        case DECIMAL_ELEM: 
        case REAL_ELEM: 
        case STRING_ELEM: 
        case LIST_ELEM: 
        case TREE_ELEM: 
        case HASHTAB_ELEM: 
        case STACK_ELEM:
        case BIGINT_ELEM:
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

extern int8_t cmp_stack(Stack *x, Stack *y) {
    if (x->type != y->type) {
        return -1;
    }
    if (x->size != y->size) {
        return -2;
    }
    if (x->pointer != y->pointer) {
        return -3;
    }
    for (size_t i = 0; i < x->pointer; ++i) {
        _Bool fval = 0;
        switch(x->type) {
            case DECIMAL_ELEM:
                fval = x->buffer[i].decimal == y->buffer[i].decimal;
            break;
            case REAL_ELEM:
                fval = x->buffer[i].real == y->buffer[i].real;
            break;
            case STRING_ELEM:
                fval = strcmp(x->buffer[i].string, y->buffer[i].string) == 0;
            break;
            case LIST_ELEM:
                fval = cmp_list(x->buffer[i].list, y->buffer[i].list) == 0;
            break;
            case TREE_ELEM:
                fval = cmp_tree(x->buffer[i].tree, y->buffer[i].tree) == 0;
            break;
            case HASHTAB_ELEM:
                fval = cmp_hashtab(x->buffer[i].hashtab, y->buffer[i].hashtab) == 0;
            break;
            case STACK_ELEM:
                fval = cmp_stack(x->buffer[i].stack, y->buffer[i].stack) == 0;
            break;
            case BIGINT_ELEM:
                fval = cmp_bigint(x->buffer[i].bigint, y->buffer[i].bigint) == 0;
            break;
        }
        if (!fval) {
            return 1;
        }
    }
    return 0;
}

extern size_t size_stack(Stack *stack) {
    return stack->size;
}

extern size_t sizeof_stack(void) {
    return sizeof(Stack);
}

extern void print_stack(Stack *stack) {
    printf("#S[ ");
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
            case LIST_ELEM:
                print_list(stack->buffer[i].list);
                putchar(' ');
            break;
            case TREE_ELEM:
                print_tree(stack->buffer[i].tree);
                putchar(' ');
            break;
            case HASHTAB_ELEM:
                print_hashtab(stack->buffer[i].hashtab);
                putchar(' ');
            break;
            case STACK_ELEM:
                print_stack(stack->buffer[i].stack);
                putchar(' ');
            break;
            case BIGINT_ELEM:
                print_bigint(stack->buffer[i].bigint);
                putchar(' ');
            break;
        }
    }
    putchar(']');
}

extern void println_stack(Stack *stack) {
    print_stack(stack);
    putchar('\n');
}

extern int8_t push_stack(Stack *stack, void *value) {
    if (stack->pointer == stack->size) {
        fprintf(stderr, "%s\n", "stack overflow");
        return 1;
    }
    switch(stack->type) {
        case DECIMAL_ELEM:
            stack->buffer[stack->pointer++].decimal = (int32_t)(intptr_t)value;
        break;
        case REAL_ELEM:
            stack->buffer[stack->pointer++].real = *(double*)value;
            free((double*)value);
        break;
        case STRING_ELEM:
            stack->buffer[stack->pointer++].string = (uint8_t*)value;
        break;
        case LIST_ELEM:
            stack->buffer[stack->pointer++].list = (struct List*)value;
        break;
        case TREE_ELEM:
            stack->buffer[stack->pointer++].tree = (struct Tree*)value;
        break;
        case HASHTAB_ELEM:
            stack->buffer[stack->pointer++].hashtab = (struct HashTab*)value;
        break;
        case STACK_ELEM:
            stack->buffer[stack->pointer++].stack = (struct Stack*)value;
        break;
        case BIGINT_ELEM:
            stack->buffer[stack->pointer++].bigint = (struct BigInt*)value;
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
    switch(stack->type) {
        case LIST_ELEM: case TREE_ELEM: case HASHTAB_ELEM: case STACK_ELEM: case BIGINT_ELEM:
            _free_stack(stack);
        break;
    }
    free(stack->buffer);
    free(stack);
}

static void _free_stack(Stack *stack) {
    for (size_t i = 0; i < stack->pointer; ++i) {
        switch(stack->type) {
            case LIST_ELEM: 
                free_list(stack->buffer[i].list);
            break;
            case TREE_ELEM: 
                free_tree(stack->buffer[i].tree);
            break;
            case HASHTAB_ELEM: 
                free_hashtab(stack->buffer[i].hashtab);
            break;
            case STACK_ELEM: 
                free_stack(stack->buffer[i].stack);
            break;
            case BIGINT_ELEM:
                free_bigint(stack->buffer[i].bigint);
            break;
        }
    }
}
