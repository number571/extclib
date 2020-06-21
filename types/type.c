#include <stdint.h>
#include <stdlib.h>

#include "array.h"
#include "bigint.h"
#include "dynamic.h"
#include "hashtab.h"
#include "list.h"
#include "tree.h"
#include "type.h"

extern void *decimal(int32_t x) {
    return (void*)(intptr_t)x;
}

extern void *string(char *x) {
    return (void*)x;
}

extern void *real(double x) {
    double *f = (double*)malloc(sizeof(double));
    *f = x;
    return (void*)f;
}
