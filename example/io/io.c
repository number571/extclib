#include <stdio.h>

#include "extclib/io.h"

#define BUFF_SIZE 16

// gcc main.c extclib/io.c -o main

int main(void) {
    uint8_t buffer[BUFF_SIZE];
    input_string(buffer, BUFF_SIZE);
    printf("%s\n", buffer);
    return 0;
}
