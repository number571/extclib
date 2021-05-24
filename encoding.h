#ifndef EXTCLIB_ENCODING_H_
#define EXTCLIB_ENCODING_H_

#include <stdint.h>

extern uint64_t join_8bits_to_64bits(const uint8_t b8[8]);
extern void split_64bits_to_8bits(uint64_t b64, uint8_t b8[8]);

#endif /* EXTCLIB_ENCODING_H_ */
