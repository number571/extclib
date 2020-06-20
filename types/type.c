#include <stdint.h>
#include <stdlib.h>

#include "type.h"
#include "list.h"
#include "tree.h"
#include "hashtab.h"
#include "array.h"
#include "bigint.h"
#include "dynamic.h"

extern void *decimal(int32_t x) {
    return (void*)(intptr_t)x;
}

extern void *chars(char *x) {
    return (void*)x;
}

extern void *real(double x) {
    double *f = (double*)malloc(sizeof(double));
    *f = x;
    return (void*)f;
}

extern void *list(struct List *x) {
    return (void*)x;
}

extern void *tree(struct Tree *x) {
    return (void*)x;
}

extern void *hashtab(struct HashTab *x) {
    return (void*)x;
}

extern void *bigint(struct BigInt *x) {
    return (void*)x;
}

extern void *array(struct Array *x) {
    return (void*)x;
}

extern void *dynamic(struct Dynamic *x) {
    return (void*)x;
}
