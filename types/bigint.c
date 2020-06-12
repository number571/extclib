#include <stdint.h>
#include <stdlib.h>

#include "bigint/bigd.h"

typedef BIGD BigInt;

extern BigInt *new_bigint(char *str) {
    BigInt *bignum = (BigInt*)malloc(sizeof(BigInt));
    *bignum = bdNew();
    bdConvFromDecimal(*bignum, str);
    return bignum;
}

extern void free_bigint(BigInt *x) {
    bdFree(x);
    free(x);
}

extern void mul_bigint(BigInt *z, BigInt *x, BigInt *y) {
    bdMultiply_s(*z, *x, *y);
}

extern void div_bigint(BigInt *z, BigInt *x, BigInt *y) {
    BIGD a = bdNew();
    BIGD b = bdNew();
    bdDivide(a, b, *x, *y);
    bdSetEqual(*z, a);
    bdFree(&b);
    bdFree(&a);
}

extern void mod_bigint(BigInt *z, BigInt *x, BigInt *y) {
    BIGD a = bdNew();
    BIGD b = bdNew();
    bdDivide(a, b, *x, *y);
    bdSetEqual(*z, b);
    bdFree(&b);
    bdFree(&a);
}

extern void expmod_bigint(BigInt *z, BigInt *x, BigInt *e, BigInt *m) {
    bdModExp(*z, *x, *e, *m);
}

extern void divmod_bigint(BigInt *q, BigInt *r, BigInt *x, BigInt *y) {
    bdDivide_s(*q, *r, *x, *y);
}

extern void add_bigint(BigInt *z, BigInt *x, BigInt *y) {
    bdAdd_s(*z, *x, *y);
}

extern void sub_bigint(BigInt *z, BigInt *x, BigInt *y) {
    bdSubtract_s(*z, *x, *y);
}

extern void inv_bigint(BigInt *z, BigInt *a, BigInt *b) {
    bdModInv(*z, *a, *b);
}

extern void shl_bigint(BigInt *z, BigInt *x, size_t q) {
    bdShiftLeft(*z, *x, q);
}

extern void shr_bigint(BigInt *z, BigInt *x, size_t q) {
    bdShiftRight(*z, *x, q);
}

extern void xor_bigint(BigInt *z, BigInt *x, BigInt *y) {
    bdXorBits(*z, *x, *y);
}

extern void and_bigint(BigInt *z, BigInt *x, BigInt *y) {
    bdAndBits(*z, *x, *y);
}

extern void or_bigint(BigInt *z, BigInt *x, BigInt *y) {
    bdOrBits(*z, *x, *y);
}

extern void not_bigint(BigInt *z, BigInt *x) {
    bdNotBits(*z, *x);
}

extern void cpynum_bigint(BigInt *x, uint32_t y) {
    bdSetShort(*x, y);
}

extern void cpystr_bigint(BigInt *x, char *y) {
    bdConvFromDecimal(*x, y);
}

extern BigInt *load_bigint(BigInt *x) {
    BigInt *bignum = (BigInt*)malloc(sizeof(BigInt));
    *bignum = bdNew();
    bdSetEqual(*bignum, *x);
    return bignum;
}

extern uint32_t getnum_bigint(BigInt *x) {
    return bdToShort(*x);
}

extern size_t sizeof_bigint(void) {
    return sizeof(BigInt);
}

extern void cpy_bigint(BigInt *x, BigInt *y) {
    bdSetEqual(*x, *y);
}

extern int8_t cmp_bigint(BigInt *x, BigInt *y) {
    int cond = bdCompare(*x, *y);
    if (cond > 0) {
        return 1;
    } else if (cond < 0) {
        return -1;
    }
    return 0;
}

extern int8_t cmpnum_bigint(BigInt *x, uint32_t y) {
    int cond = bdShortCmp(*x, y);
    if (cond > 0) {
        return 1;
    } else if (cond < 0) {
        return -1;
    }
    return 0;
}

extern void print_bigint(BigInt *x) {
    bdPrintDecimal("", *x, "");
}

extern void println_bigint(BigInt *x) {
    bdPrintDecimal("", *x, "\n");
}
