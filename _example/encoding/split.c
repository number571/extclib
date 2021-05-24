#include <stdio.h>

#include "extclib/encoding.h"

int main(void) {
	uint8_t arr[8];
	uint64_t x = 0xAABBCC;
	split_64bits_to_8bits(x, arr);
	for (int i = 0; i < 8; ++i) {
		printf("%02X\n", arr[i]);
	}
    return 0;
}
