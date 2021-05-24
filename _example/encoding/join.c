#include <stdio.h>

#include "extclib/encoding.h"

int main(void) {
	uint8_t arr[8] = { 0xAA, 0xBB, 0xCC };
	uint64_t x = join_8bits_to_64bits(arr);
    printf("%016llX\n", x);
    return 0;
}
