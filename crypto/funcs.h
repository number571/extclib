#ifndef EXTCLIB_CRYPTO_FUNCS_H_
#define EXTCLIB_CRYPTO_FUNCS_H_

#include <stdint.h>

extern uint8_t gf8_mul(uint8_t a, uint8_t b);
extern uint32_t join_8bits_to_32bits(uint8_t * blocks8b);
extern void split_32bits_to_8bits(uint32_t block32b, uint8_t * blocks8b);

#endif /* EXTCLIB_CRYPTO_FUNCS_H_ */
