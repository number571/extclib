#ifndef EXTCLIB_TYPE_H_
#define EXTCLIB_TYPE_H_

#include <stdint.h>

typedef enum vtype_t {
    DECIMAL_TYPE,
    REAL_TYPE,
    CHARS_TYPE,
    LIST_TYPE,
    TREE_TYPE,
    HASHTAB_TYPE,
    ARRAY_TYPE,
    BIGINT_TYPE,
    DYNAMIC_TYPE,
} vtype_t;

typedef union value_t {
    int32_t decimal;
    double real;
    char *chars;
    struct List *list;
    struct Tree *tree;
    struct HashTab *hashtab;
    struct Array *array;
    struct BigInt *bigint;
    struct Dynamic *dynamic;
} value_t;

extern void *decimal(int32_t x);
extern void *chars(char *x);
extern void *real(double x);
extern void *list(struct List *x);
extern void *tree(struct Tree *x);
extern void *hashtab(struct HashTab *x);
extern void *bigint(struct BigInt *x);
extern void *array(struct Array *x);
extern void *dynamic(struct Dynamic *x);

#endif /* EXTCLIB_TYPE_H_ */
