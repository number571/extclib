#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extclib/net.h"

#define BUFFSIZE 64

void sendhtml(int conn, char *filename);

int main(void) {
    int listener = listen_net("127.0.0.1:7545");
    if (listener < 0) {
        return 1;
    }
    while(1) {
        int conn = accept_net(listener);
        if (conn < 0) {
            continue;
        }
        while(1) {
            char buffer[BUFFSIZE] = {0};
            int n = recv_net(conn, buffer, BUFFSIZE);
            if (n < 0) {
                break;
            }
            printf("%.*s", n, buffer);
            if (n != BUFFSIZE) {
                break;
            }
        }
        sendhtml(conn, "index.html");
        close_net(conn);
    }
    close_net(listener);
    return 0;
}

void sendhtml(int conn, char *filename) {
    char buffer[BUFFSIZE] = "HTTP/1.1 200 OK\r\n\n";
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    while((readsize = fread(buffer, sizeof(char), BUFFSIZE, file)) != 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}
