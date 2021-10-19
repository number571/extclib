#ifndef EXTCLIB_MATH_RATIONAL_H_
#define EXTCLIB_MATH_RATIONAL_H_

#include "bigint.h"

typedef struct rational_t rational_t;

extern rational_t *rational_new(bigint_t *n, bigint_t *d);
extern void rational_free(rational_t *r);

extern void rational_cpy(rational_t *r, rational_t *x);
extern void rational_out(rational_t *r, FILE *stream);

extern int rational_cmp(rational_t *x, rational_t *y);
extern void rational_add(rational_t *r, rational_t *x, rational_t *y);
extern void rational_sub(rational_t *r, rational_t *x, rational_t *y);
extern void rational_mul(rational_t *r, rational_t *x, rational_t *y);
extern void rational_div(rational_t *r, rational_t *x, rational_t *y);

#endif /* EXTCLIB_MATH_RATIONAL_H_ */
