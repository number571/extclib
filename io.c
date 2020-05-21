#include <stdio.h>

#include "io.h"

#define INDEX(ptr, init) (ptr-init)

extern void input_string(uint8_t *buffer, size_t size) {
	if (size == 0) {
		return;
	}
    uint8_t *ptr = buffer;
    while((*ptr = getchar()) != '\n' && INDEX(ptr, buffer) < (size-1)) {
        ++ptr;
    }
    *ptr = '\0';
}
