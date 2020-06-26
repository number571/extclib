#ifndef EXTCLIB_HTTP_H_
#define EXTCLIB_HTTP_H_

#include <stdint.h>

typedef struct Request Request;
typedef struct HTTP HTTP;

extern HTTP *new_http(char *address);
extern void free_http(HTTP *http);

extern int8_t handle_http(HTTP *http, char *path, void(*handle)(int conn, Request *req));
extern int8_t listen_http(HTTP *http);

extern void parsehtml_http(int conn, char *filename);

#endif /* EXTCLIB_HTTP_H_ */
