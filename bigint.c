#include <stdint.h>
#include <stdlib.h>

#include "bigint/bigd.h"

typedef BIGD BigInt;

extern BigInt *new_bigint(uint8_t *str) {
    BigInt *bignum = (BigInt*)malloc(sizeof(BigInt));
    *bignum = bdNew();
    bdConvFromDecimal(*bignum, (char*)str);
    return bignum;
}

extern void free_bigint(BigInt *x) {
    bdFree(x);
    free(x);
}

extern void mul_bigint(BigInt *x, BigInt *y) {
    BIGD a = bdNew();
    bdSetEqual(a, *x);
    bdMultiply(*x, a, *y);
    bdFree(&a);
}

extern void div_bigint(BigInt *x, BigInt *y) {
    BIGD a = bdNew();
    BIGD b = bdNew();
    bdSetEqual(a, *x);
    bdDivide(*x, b, a, *y);
    bdFree(&a);
    bdFree(&b);
}

extern void mod_bigint(BigInt *x, BigInt *y) {
    BIGD a = bdNew();
    BIGD b = bdNew();
    bdSetEqual(a, *x);
    bdDivide(b, *x, a, *y);
    bdFree(&a);
    bdFree(&b);
}

extern void add_bigint(BigInt *x, BigInt *y) {
    bdAdd(*x, *x, *y);
}

extern void sub_bigint(BigInt *x, BigInt *y) {
    bdSubtract(*x, *x, *y);
}

extern void shl_bigint(BigInt *x, size_t q) {
    bdShiftLeft(*x, *x, q);
}

extern void shr_bigint(BigInt *x, size_t q) {
    bdShiftRight(*x, *x, q);
}

extern void xor_bigint(BigInt *x, BigInt *y) {
    bdXorBits(*x, *x, *y);
}

extern void and_bigint(BigInt *x, BigInt *y) {
    bdAndBits(*x, *x, *y);
}

extern void or_bigint(BigInt *x, BigInt *y) {
    bdOrBits(*x, *x, *y);
}

extern void not_bigint(BigInt *x) {
    bdNotBits(*x, *x);
}

extern void cpy_bigint(BigInt *x, BigInt *y) {
    bdSetEqual(*x, *y);
}

extern int8_t cmp_bigint(BigInt *x, BigInt *y) {
    return (int8_t)bdCompare(*x, *y);
}

extern void print_bigint(BigInt *x) {
    bdPrintDecimal("", *x, "\n");
}