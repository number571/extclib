#include "bigint.h"
#include "bigint/mini-mpq.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct bigint_t {
    mpz_t decimal;
} bigint_t;

extern bigint_t *bigint_new(char *str) {
	bigint_t *num = (bigint_t*)malloc(sizeof(bigint_t));
    mpz_init(num->decimal);
    if(str[0] == '0') {
    	switch(str[1]) {
    		case 'b':
    			mpz_set_str(num->decimal, str+2, 2);
    		break;
    		case 'o':
    			mpz_set_str(num->decimal, str+2, 8);
    		break;
    		case 'x':
    			mpz_set_str(num->decimal, str+2, 16);
    		break;
    	}
    } else {
    	mpz_set_str(num->decimal, str, 10);
    }
    return num;
}

extern void bigint_free(bigint_t *num) {
	mpz_clear(num->decimal);
    free(num);
}



extern void bigint_cpy(bigint_t *r, bigint_t *x) {
	mpz_set(r->decimal, x->decimal);
}

extern void bigint_out(bigint_t *num, FILE *stream, int base) {
    mpz_out_str(stream, base, num->decimal);
}

extern void bigint_load(bigint_t *num, const unsigned char *bytes, int size) {
    mpz_import(num->decimal, size, -1, sizeof(char), 0, 0, bytes);
}

extern int bigint_save(bigint_t *num, unsigned char *output, int size) {
    size_t val = (size_t)size;
    mpz_export(output, &val, -1, sizeof(char), 0, 0, num->decimal);
    return (int)val;
}



extern int bigint_isprime(bigint_t *r) {
    return mpz_probab_prime_p(r->decimal, 15);
}

extern void bigint_gcd(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_gcd(r->decimal, x->decimal, y->decimal);
}

extern void bigint_inv(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_invert(r->decimal, x->decimal, y->decimal);
}

extern void bigint_expmod(bigint_t *r, bigint_t *x, bigint_t *e, bigint_t *n) {
    mpz_powm(r->decimal, x->decimal, e->decimal, n->decimal);
}



extern void bigint_shl(bigint_t *r, bigint_t *x, unsigned q) {
    mpz_mul_2exp(r->decimal, x->decimal, q);
}

extern void bigint_shr(bigint_t *r, bigint_t *x, unsigned q) {
    mpz_fdiv_q_2exp(r->decimal, x->decimal, q);
}

extern void bigint_xor(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_xor(r->decimal, x->decimal, y->decimal);
}

extern void bigint_and(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_and(r->decimal, x->decimal, y->decimal);
}

extern void bigint_or(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_ior(r->decimal, x->decimal, y->decimal);
}

extern void bigint_not(bigint_t *r, bigint_t *x) {
    mpz_com(r->decimal, x->decimal);
}



extern int bigint_cmp(bigint_t *x, bigint_t *y) {
	return mpz_cmp(x->decimal, y->decimal);
}

extern void bigint_add(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_add(r->decimal, x->decimal, y->decimal);
}

extern void bigint_sub(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_sub(r->decimal, x->decimal, y->decimal);
}

extern void bigint_mul(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_mul(r->decimal, x->decimal, y->decimal);
}

extern void bigint_div(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_fdiv_q(r->decimal, x->decimal, y->decimal);
}

extern void bigint_mod(bigint_t *r, bigint_t *x, bigint_t *y) {
    mpz_fdiv_r(r->decimal, x->decimal, y->decimal);
}

extern void bigint_exp(bigint_t *r, bigint_t *x, unsigned e) {
    mpz_pow_ui(r->decimal, x->decimal, e);
}

extern void bigint_neg(bigint_t *r, bigint_t *x) {
	mpz_neg(r->decimal, x->decimal);
}

extern void bigint_abs(bigint_t *r, bigint_t *x) {
	mpz_abs(r->decimal, x->decimal);
}


extern int bigint_cmp_ui(bigint_t *x, unsigned y) {
	return mpz_cmp_ui(x->decimal, y);
}

extern void bigint_add_ui(bigint_t *r, bigint_t *x, unsigned y) {
    mpz_add_ui(r->decimal, x->decimal, y);
}

extern void bigint_sub_ui(bigint_t *r, bigint_t *x, unsigned y) {
    mpz_sub_ui(r->decimal, x->decimal, y);
}

extern void bigint_mul_ui(bigint_t *r, bigint_t *x, unsigned y) {
    mpz_mul_ui(r->decimal, x->decimal, y);
}

extern void bigint_div_ui(bigint_t *r, bigint_t *x, unsigned y) {
    mpz_fdiv_q_ui(r->decimal, x->decimal, y);
}

extern void bigint_mod_ui(bigint_t *r, bigint_t *x, unsigned y) {
    mpz_fdiv_r_ui(r->decimal, x->decimal, y);
}
