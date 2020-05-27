#ifndef EXTCLIB_BIGINT_H_
#define EXTCLIB_BIGINT_H_

#include <stdint.h>

typedef struct BigInt BigInt;

extern BigInt *new_bigint(uint8_t *str);
extern void free_bigint(BigInt *x);

extern void mul_bigint(BigInt *x, BigInt *y);
extern void div_bigint(BigInt *x, BigInt *y);
extern void mod_bigint(BigInt *x, BigInt *y);

extern void add_bigint(BigInt *x, BigInt *y);
extern void sub_bigint(BigInt *x, BigInt *y);

extern void shl_bigint(BigInt *x, size_t q);
extern void shr_bigint(BigInt *x, size_t q);

extern void xor_bigint(BigInt *x, BigInt *y);
extern void and_bigint(BigInt *x, BigInt *y);
extern void or_bigint(BigInt *x, BigInt *y);
extern void not_bigint(BigInt *x);

extern void cpy_bigint(BigInt *x, BigInt *y);
extern int8_t cmp_bigint(BigInt *x, BigInt *y);

extern void print_bigint(BigInt *x);
extern void println_bigint(BigInt *x);

#endif /* EXTCLIB_BIGINT_H_ */
