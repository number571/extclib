#include <stdint.h>
#include <stdlib.h>

extern void *decimal(int32_t x) {
    return (void*)(intptr_t)x;
}

extern void *string(uint8_t *x) {
    return (void*)x;
}

extern void *real(double x) {
    double *f = (double*)malloc(sizeof(double));
    *f = x;
    return (void*)f;
}
