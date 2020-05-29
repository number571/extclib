#include <stdio.h>
#include "extclib/io.h"

#define BUFF_SIZE 16

int main(void) {
    uint8_t buffer[BUFF_SIZE];
    inputs_io(buffer, BUFF_SIZE);
    printf("%s\n", buffer);
    return 0;
}
