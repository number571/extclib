#include <stdint.h>

extern uint64_t join_8bits_to_64bits(const uint8_t b8[8]) {
	uint64_t b64;
	for (int i = 0; i < 8; ++i) {
		b64 = (b64 << 8) | b8[i];
	}
	return b64;
}

extern void split_64bits_to_8bits(uint64_t b64, uint8_t b8[8]) {
	for (size_t i = 0; i < 8; ++i) {
		b8[i] = (uint8_t)(b64 >> ((7 - i) * 8));
	}
}
