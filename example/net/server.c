#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "extclib/net.h"

#define BUFF 256

// gcc main.c extclib/net.c -o main

int main (void) {
    int listener = tcp_listen("0.0.0.0:8080");
    if (listener < 0) {
        fprintf(stderr, "%d\n", listener);
        return 1;
    }

    printf("Server is listening ...\n");
    uint8_t buffer[BUFF];

    while (1) {
        int conn = tcp_accept(listener);
        if (conn < 0) {
            fprintf(stderr, "Error: accept\n");
            return 3;
        }
        while (1) {
            int length = tcp_recv(conn, buffer, BUFF);
            if (length <= 0) {
                break;
            }
            for (uint8_t *p = buffer; *p != '\0'; ++p) {
                *p = toupper(*p);
            }
            tcp_send(conn, buffer, BUFF);
        }
        tcp_close(conn);
    }

    tcp_close(listener);
    return 0;
}
