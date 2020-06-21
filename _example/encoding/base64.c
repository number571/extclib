#include <stdio.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/encoding.h"

#define BUFFSIZE 64

int main(void) {
    uint8_t in[BUFFSIZE] = "hello, world!";
    uint8_t out[BUFFSIZE] = {0};
    
    encode_base64((Context){
        .data = {
            .in = in,
            .out = out,
            .size = strlen((char*)in),
        },
    });
    printf("%s\n", out);

    memset(in, 0, BUFFSIZE);

    decode_base64((Context){
        .data = {
            .in = out,
            .out = in,
            .size = strlen((char*)out),
        },
    });
    printf("%s\n", in);
    return 0;
}
