#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "rational.h"

typedef struct rational_t {
    bigint_t *numer;
    bigint_t *denom;
} rational_t;

static void _gcd(bigint_t *r, bigint_t *x, bigint_t *y);

extern rational_t *rational_new(bigint_t *n, bigint_t *d) {
    rational_t *r;
    bigint_t *gcdv;

    if (bigint_cmp_ui(d, 0) == 0) {
        return NULL;
    }

    gcdv = bigint_new("0");
    _gcd(gcdv, n, d);

    bigint_div(n, n, gcdv);
    bigint_div(d, d, gcdv);

    bigint_free(gcdv);

    r = (rational_t*)malloc(sizeof(rational_t));
    r->numer = n;
    r->denom = d;

    return r;
}

extern void rational_free(rational_t *r) {
    bigint_free(r->numer);
    bigint_free(r->denom);
    free(r);
}

extern void rational_cpy(rational_t *r, rational_t *x) {
    bigint_cpy(r->numer, x->numer);
    bigint_cpy(r->denom, x->denom);
}

extern void rational_out(rational_t *r, FILE *stream) {
    bigint_out(r->numer, stream, 10);
    fputc('/', stream);
    bigint_out(r->denom, stream, 10);
}

extern int rational_cmp(rational_t *x, rational_t *y) {
    bigint_t *ls, *rs;
    int flag;

    if (bigint_cmp(x->numer, y->numer) == 0 && bigint_cmp(x->denom, y->denom) == 0) {
        return 0;
    }

    ls = bigint_new("0");
    rs = bigint_new("0");

    bigint_mul(ls, x->numer, y->denom);
    bigint_mul(rs, y->denom, y->numer);

    if (bigint_cmp(ls, rs) < 0) {
        flag = -1;
    } else {
        flag = 1;
    }

    bigint_free(ls);
    bigint_free(rs);

    return flag;
}

extern void rational_add(rational_t *r, rational_t *x, rational_t *y) {
    bigint_t *tmp, *num, *den, *txn, *tyn, *txd, *tyd;
    
    tmp = bigint_new("0");
    num = bigint_new("0");
    den = bigint_new("0");

    txn = bigint_new("0");
    tyn = bigint_new("0");
    txd = bigint_new("0");
    tyd = bigint_new("0");

    bigint_cpy(txn, x->numer);
    bigint_cpy(tyn, y->numer);
    bigint_cpy(txd, x->denom);
    bigint_cpy(tyd, y->denom);

    // num = x.numer * y.denom + x.numer * y.denom
    bigint_mul(num, txn, tyd);
    bigint_mul(tmp, tyn, txd);
    bigint_add(num, num, tmp);

    // den = x.denom * y.denom
    bigint_mul(den, txd, tyd);

    _gcd(tmp, num, den);

    bigint_div(r->numer, num, tmp);
    bigint_div(r->denom, den, tmp);

    bigint_free(txn);
    bigint_free(tyn);
    bigint_free(txd);
    bigint_free(tyd);

    bigint_free(tmp);
    bigint_free(num);
    bigint_free(den);
}

extern void rational_sub(rational_t *r, rational_t *x, rational_t *y) {
    bigint_t *tmp, *num, *den, *txn, *tyn, *txd, *tyd;
    
    tmp = bigint_new("0");
    num = bigint_new("0");
    den = bigint_new("0");

    txn = bigint_new("0");
    tyn = bigint_new("0");
    txd = bigint_new("0");
    tyd = bigint_new("0");

    bigint_cpy(txn, x->numer);
    bigint_cpy(tyn, y->numer);
    bigint_cpy(txd, x->denom);
    bigint_cpy(tyd, y->denom);

    // num = x.numer * y.denom - x.numer * y.denom
    bigint_mul(num, txn, tyd);
    bigint_mul(tmp, tyn, txd);
    bigint_sub(num, num, tmp);

    // den = x.denom * y.denom
    bigint_mul(den, txd, tyd);

    _gcd(tmp, num, den);

    bigint_div(r->numer, num, tmp);
    bigint_div(r->denom, den, tmp);

    bigint_free(txn);
    bigint_free(tyn);
    bigint_free(txd);
    bigint_free(tyd);

    bigint_free(tmp);
    bigint_free(num);
    bigint_free(den);
}

extern void rational_mul(rational_t *r, rational_t *x, rational_t *y) {
    bigint_t *tmp, *num, *den, *txn, *tyn, *txd, *tyd;
    
    tmp = bigint_new("0");
    num = bigint_new("0");
    den = bigint_new("0");

    txn = bigint_new("0");
    tyn = bigint_new("0");
    txd = bigint_new("0");
    tyd = bigint_new("0");

    bigint_cpy(txn, x->numer);
    bigint_cpy(tyn, y->numer);
    bigint_cpy(txd, x->denom);
    bigint_cpy(tyd, y->denom);

    // num = x.numer * y.numer
    bigint_mul(num, txn, tyn);

    // den = x.denom * y.denom
    bigint_mul(den, txd, tyd);

    _gcd(tmp, num, den);

    bigint_div(r->numer, num, tmp);
    bigint_div(r->denom, den, tmp);

    bigint_free(txn);
    bigint_free(tyn);
    bigint_free(txd);
    bigint_free(tyd);

    bigint_free(tmp);
    bigint_free(num);
    bigint_free(den);
}

extern void rational_div(rational_t *r, rational_t *x, rational_t *y) {
    bigint_t *tmp, *num, *den, *txn, *tyn, *txd, *tyd;
    
    tmp = bigint_new("0");
    num = bigint_new("0");
    den = bigint_new("0");

    txn = bigint_new("0");
    tyn = bigint_new("0");
    txd = bigint_new("0");
    tyd = bigint_new("0");

    bigint_cpy(txn, x->numer);
    bigint_cpy(tyn, y->numer);
    bigint_cpy(txd, x->denom);
    bigint_cpy(tyd, y->denom);

    // num = x.numer * y.denom
    bigint_mul(num, txn, tyd);

    // den = x.denom * y.numer
    bigint_mul(den, txd, tyn);

    _gcd(tmp, num, den);

    bigint_div(r->numer, num, tmp);
    bigint_div(r->denom, den, tmp);

    bigint_free(txn);
    bigint_free(tyn);
    bigint_free(txd);
    bigint_free(tyd);

    bigint_free(tmp);
    bigint_free(num);
    bigint_free(den);
}

static void _gcd(bigint_t *r, bigint_t *a, bigint_t *b) {
    bigint_t *ta, *tb;

    ta = bigint_new("0");
    tb = bigint_new("0");

    bigint_cpy(ta, a);
    bigint_cpy(tb, b);

    while(bigint_cmp_ui(tb, 0) != 0) {
        bigint_cpy(r, ta);
        bigint_cpy(ta, tb);
        bigint_mod(tb, r, tb);
    }

    bigint_cpy(r, ta);

    bigint_free(ta);
    bigint_free(tb);
}
