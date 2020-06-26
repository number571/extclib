#ifndef EXTCLIB_HTTP_H_
#define EXTCLIB_HTTP_H_

#include <stdint.h>

typedef struct HTTPreq {
    char method[16];
    char path[2048];
    char proto[16];
    uint8_t state;
    size_t index;
} HTTPreq;

typedef struct HTTP HTTP;

extern HTTP *new_http(char *address);
extern void free_http(HTTP *http);

extern int8_t handle_http(HTTP *http, char *path, void(*)(int, HTTPreq*));
extern int8_t listen_http(HTTP *http);

extern void parsehtml_http(int conn, char *filename);

#endif /* EXTCLIB_HTTP_H_ */
