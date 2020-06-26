#ifdef __linux__
    #include <unistd.h>
    #include <arpa/inet.h>
#elif __WIN32
    #include <winsock2.h>
#else
    #warning "net.h: platform not supported"
#endif

#if defined(__linux__) || defined(__WIN32)

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "net.h"

#define INDEX(ptr, init) (ptr-init)

typedef enum error_t {
    SOCKET_ERR  = -1,
    SETOPT_ERR  = -2,
    PARSE_ERR   = -3,
    CONNECT_ERR = -4,
    BIND_ERR    = -5,
    LISTEN_ERR  = -6,
    WINSOCK_ERR = -7,
} error_t;

static int8_t _parse_address(char *address, char *ipv4, char *port);

extern int listen_net(char *address) {
#ifdef __WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        return WINSOCK_ERR;
    }
#endif
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        return SOCKET_ERR;
    }
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        return SETOPT_ERR;
    }
    char ipv4[16];
    char port[6];
    if (_parse_address(address, ipv4, port) != 0) {
        return PARSE_ERR;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ipv4);
    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr))) {
        return BIND_ERR;
    }
    if (listen(listener, SOMAXCONN) != 0) {
        return LISTEN_ERR;
    }
    return listener;
}

extern int connect_net(char *address) {
#ifdef __WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        return WINSOCK_ERR;
    }
#endif
    int conn = socket(AF_INET, SOCK_STREAM, 0);
    if (conn < 0) {
        return SOCKET_ERR;
    }
    char ipv4[16];
    char port[6];
    if (_parse_address(address, ipv4, port) != 0) {
        return PARSE_ERR;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi((char*)port));
    addr.sin_addr.s_addr = inet_addr((char*)ipv4);
    if (connect(conn, (struct sockaddr *)&addr, sizeof(addr))) {
        return CONNECT_ERR;
    }
    return conn;
}

extern int accept_net(int listener) {
    return accept(listener, NULL, NULL);
}

extern int send_net(int conn, char *buffer, size_t size) {
    return send(conn, buffer, (int)size, 0);
}

extern int recv_net(int conn, char *buffer, size_t size) {
    return recv(conn, buffer, (int)size, 0);
}

extern int close_net(int conn) {
    shutdown(conn, SHUT_RDWR); 
#ifdef __linux__
    return close(conn);
#elif __WIN32
    return closesocket(conn);
#endif
}

static int8_t _parse_address(char *address, char *ipv4, char *port) {
    char *ptr = address;
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

#endif /* defined(__linux__) || defined(__WIN32) */
