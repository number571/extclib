#include <stdint.h>

extern uint8_t gf8_mul(uint8_t a, uint8_t b) {
    uint8_t hi_bit_set, p = 0;
    for(uint8_t counter = 0; counter < 8; counter++) {
        if(b & 0x01) {
            p ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if(hi_bit_set == 0x80)  {
            a ^= 0x1B;      
        }
        b >>= 1;
    }
    return p;
}

extern uint32_t join_8bits_to_32bits(uint8_t * blocks8b) {
    uint32_t block32b;
    for (uint8_t *p = blocks8b; p < blocks8b + 4; ++p) {
        block32b = (block32b << 8) | *p;
    }
    return block32b;
}

extern void split_32bits_to_8bits(uint32_t block32b, uint8_t * blocks8b) {
    for (uint8_t i = 0; i < 4; ++i) {
        blocks8b[i] = (uint8_t)(block32b >> (24 - i * 8));
    }
}
