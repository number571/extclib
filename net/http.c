#include "http.h"

#include <stdio.h>
#include <string.h>

#define BUFSIZ_4K (4 << 10)

extern int http_get(net_t *state, const char *path) {
	char buffer[BUFSIZ_4K];
	snprintf(buffer, BUFSIZ_4K, 
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n\r\n", path, net_addr(state));
	return net_send(state, buffer, strlen(buffer));
}

extern int http_post(net_t *state, const char *path, const char *data) {
	char buffer[BUFSIZ_4K];
	int dlen = strlen(data);
	snprintf(buffer, BUFSIZ_4K,
		"POST %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: %d\r\n\r\n", path, net_addr(state), dlen);
	net_send(state, buffer, strlen(buffer));
	return net_send(state, data, dlen);
}
