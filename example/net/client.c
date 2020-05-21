#include <stdio.h>
#include <stdint.h>

#include "extclib/net.h"
#include "extclib/io.h"

#define BUFF_SIZE 256

// gcc main.c extclib/net.c extclib/io.c -o main

int main(void) {
    int conn = tcp_connect("127.0.0.1:8080");
    if (conn < 0) {
        fprintf(stderr, "%d\n", conn);
        return 1;
    }

    char buffer[BUFF_SIZE];
    input_string(buffer, BUFF_SIZE);

    tcp_send(conn, buffer, BUFF_SIZE);
    tcp_recv(conn, buffer, BUFF_SIZE);

    printf("%s\n", buffer);
    tcp_close(conn);

    return 0;
}
