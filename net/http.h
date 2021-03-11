#ifndef EXTCLIB_NET_HTTP_H
#define EXTCLIB_NET_HTTP_H

#include "../net.h"

extern int http_get(net_t *state, const char *path);
extern int http_post(net_t *state, const char *path, const char *data);

#endif /* EXTCLIB_NET_HTTP_H */
