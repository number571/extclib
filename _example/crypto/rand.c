#include <stdint.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BUFFSIZE 20

int main(void) {
    uint8_t buffer[BUFFSIZE] = {0};
    Context ctx = {
        .data = {
            .size = BUFFSIZE,
            .out = buffer,
        },
    };
    for (size_t i = 0; i < 10; ++i) {
        bytes_rand(ctx);
        printf_io("%$\n", buffer, BUFFSIZE);
    }
    return 0;
}
