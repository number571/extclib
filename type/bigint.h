#ifndef EXTCLIB_TYPE_BIGINT_H_
#define EXTCLIB_TYPE_BIGINT_H_

#include <stdio.h>

typedef struct bigint_t bigint_t;

extern bigint_t *bigint_new(char *str);
extern void bigint_free(bigint_t *num);

extern void bigint_cpy(bigint_t *r, bigint_t *x);
extern void bigint_out(bigint_t *num, FILE *stream, int base);
extern void bigint_load(bigint_t *num, const unsigned char *bytes, int size);
extern int bigint_save(bigint_t *num, unsigned char *output, int size);

extern int bigint_isprime(bigint_t *r);
extern void bigint_gcd(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_inv(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_expmod(bigint_t *r, bigint_t *x, bigint_t *e, bigint_t *n);

extern void bigint_shl(bigint_t *r, bigint_t *x, unsigned q);
extern void bigint_shr(bigint_t *r, bigint_t *x, unsigned q);
extern void bigint_xor(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_and(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_or(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_not(bigint_t *r, bigint_t *x);

extern int bigint_cmp(bigint_t *x, bigint_t *y);
extern void bigint_add(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_sub(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_mul(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_div(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_mod(bigint_t *r, bigint_t *x, bigint_t *y);
extern void bigint_neg(bigint_t *r, bigint_t *x);
extern void bigint_abs(bigint_t *r, bigint_t *x);
extern void bigint_exp(bigint_t *r, bigint_t *x, unsigned e);

extern int bigint_cmp_ui(bigint_t *x, unsigned y);
extern void bigint_add_ui(bigint_t *r, bigint_t *x, unsigned y);
extern void bigint_sub_ui(bigint_t *r, bigint_t *x, unsigned y);
extern void bigint_mul_ui(bigint_t *r, bigint_t *x, unsigned y);
extern void bigint_div_ui(bigint_t *r, bigint_t *x, unsigned y);
extern void bigint_mod_ui(bigint_t *r, bigint_t *x, unsigned y);

#endif /* EXTCLIB_TYPE_BIGINT_H_ */
