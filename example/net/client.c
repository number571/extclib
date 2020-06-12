#include <stdio.h>
#include <stdint.h>

#include "extclib/net.h"
#include "extclib/io.h"

#define BUFF_SIZE 256

int main(void) {
    int conn = connect_net((uint8_t*)"127.0.0.1:8080");
    if (conn < 0) {
        fprintf(stderr, "%d\n", conn);
        return 1;
    }

    char buffer[BUFF_SIZE];
    inputs_io((uint8_t*)buffer, BUFF_SIZE);

    send_net(conn, (uint8_t*)buffer, BUFF_SIZE);
    recv_net(conn, (uint8_t*)buffer, BUFF_SIZE);

    printf("%s\n", buffer);
    close_net(conn);

    return 0;
}
