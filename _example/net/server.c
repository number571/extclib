#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "extclib/net.h"

#define BUFF 256

int main (void) {
    int listener = listen_net("0.0.0.0:8080");
    if (listener < 0) {
        fprintf(stderr, "%d\n", listener);
        return 1;
    }

    printf("Server is listening ...\n");
    char buffer[BUFF];

    while (1) {
        int conn = accept_net(listener);
        if (conn < 0) {
            fprintf(stderr, "Error: accept\n");
            return 3;
        }
        while (1) {
            int length = recv_net(conn, buffer, BUFF);
            if (length <= 0) {
                break;
            }
            for (char *p = buffer; *p != '\0'; ++p) {
                *p = toupper(*p);
            }
            send_net(conn, buffer, BUFF);
        }
        close_net(conn);
    }

    close_net(listener);
    return 0;
}
