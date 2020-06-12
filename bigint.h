#ifndef EXTCLIB_BIGINT_H_
#define EXTCLIB_BIGINT_H_

#include <stddef.h>
#include <stdint.h>

typedef struct BigInt BigInt;

extern BigInt *new_bigint(char *str);
extern void free_bigint(BigInt *x);

extern void mul_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void div_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void mod_bigint(BigInt *z, BigInt *x, BigInt *y);

extern void expmod_bigint(BigInt *z, BigInt *x, BigInt *e, BigInt *m);
extern void divmod_bigint(BigInt *q, BigInt *r, BigInt *x, BigInt *y);

extern void add_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void sub_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void inv_bigint(BigInt *z, BigInt *a, BigInt *b);

extern void shl_bigint(BigInt *z, BigInt *x, size_t q);
extern void shr_bigint(BigInt *z, BigInt *x, size_t q);

extern void xor_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void and_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void or_bigint(BigInt *z, BigInt *x, BigInt *y);
extern void not_bigint(BigInt *z, BigInt *x);

extern BigInt *load_bigint(BigInt *x);
extern uint32_t getnum_bigint(BigInt *x);
extern size_t sizeof_bigint(void);

extern void cpy_bigint(BigInt *x, BigInt *y);
extern void cpynum_bigint(BigInt *x, uint32_t str);
extern void cpystr_bigint(BigInt *x, char *str);

extern int8_t cmp_bigint(BigInt *x, BigInt *y);
extern int8_t cmpnum_bigint(BigInt *x, uint32_t y);

extern void print_bigint(BigInt *x);
extern void println_bigint(BigInt *x);

#endif /* EXTCLIB_BIGINT_H_ */
