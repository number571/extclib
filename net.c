#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "net.h"

#define INDEX(ptr, init) (ptr-init)

static int8_t _parse_address(uint8_t *address, uint8_t *ipv4, uint8_t *port);

extern int listen_net(uint8_t *address) {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        return -1;
    }

    uint8_t ipv4[16];
    uint8_t port[6];

    if (_parse_address(address, ipv4, port) != 0) {
        return -2;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ipv4);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr))) {
        return -3;
    }

    if (listen(listener, SOMAXCONN) != 0) {
        return -4;
    }

    return listener;
}

extern int connect_net(uint8_t *address) {
    int conn = socket(AF_INET, SOCK_STREAM, 0);
    if (conn < 0) {
        return -1;
    }

    uint8_t ipv4[16];
    uint8_t port[6];

    if (_parse_address(address, ipv4, port) != 0) {
        return -2;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ipv4);

    if (connect(conn, (struct sockaddr *)&addr, sizeof(addr))) {
        return -3;
    }

    return conn;
}

extern int accept_net(int listener) {
    return accept(listener, NULL, NULL);
}

extern int send_net(int conn, uint8_t *buffer, size_t size) {
    // return send(conn, (char*)buffer, (int)size, 0);
    return write(conn, (char*)buffer, (int)size);
}

extern int recv_net(int conn, uint8_t *buffer, size_t size) {
    // return recv(conn, (char*)buffer, (int)size, 0);
    return read(conn, (char*)buffer, (int)size);
}

extern int close_net(int conn) {
    return close(conn);
}

static int8_t _parse_address(uint8_t *address, uint8_t *ipv4, uint8_t *port) {
    uint8_t *ptr = address;
    while(*ptr != ':') {
        if (*ptr == '\0') {
            return 1;
        }
        if (INDEX(ptr, address) > 15) {
            return 2;
        }
        *ipv4++ = *ptr++;
    }
    *ipv4 = '\0';
    ++ptr;
    address = ptr;
    ptr = address;
    while(*ptr != '\0') {
        if (INDEX(ptr, address) > 5) {
            return 3;
        }
        *port++ = *ptr++;
    }
    *port = '\0';
    return 0;
}
