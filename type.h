#ifndef EXTCLIB_TYPE_H_
#define EXTCLIB_TYPE_H_

#include <stdint.h>

typedef enum {
    DECIMAL_ELEM,
    REAL_ELEM,
    STRING_ELEM,
    LIST_ELEM,
    TREE_ELEM,
    HASHTAB_ELEM,
    STACK_ELEM,
    BIGINT_ELEM,
} vtype_t;

typedef union value_t {
    int32_t decimal;
    double real;
    uint8_t *string;
    struct List *list;
    struct Tree *tree;
    struct HashTab *hashtab;
    struct Stack *stack;
    struct BigInt *bigint;
} value_t;

extern void *decimal(int32_t x);
extern void *string(uint8_t *x);
extern void *real(double x);
extern void *list(struct List *x);
extern void *tree(struct Tree *x);
extern void *hashtab(struct HashTab *x);
extern void *stack(struct Stack *x);
extern void *bigint(struct BigInt *x);

#endif /* EXTCLIB_TYPE_H_ */
