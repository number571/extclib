#ifdef __unix__
	#include <unistd.h>
	#include <arpa/inet.h>
#elif __WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
#else
	#warning "platform not supported"
#endif

#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZ_1K (1 << 10)
#define BUFSIZ_4K (4 << 10)

#define ADRSIZ 256

typedef struct net_conn {
	int  conn;
	int  port;
	char addr[ADRSIZ];
} net_conn;

static int _close(int conn);
static char *_strncpy(char *output, const char *input, size_t size);

extern int net_init(void) {
	int rc = 0;
#ifdef __WIN32
	int conn = socket(AF_INET, SOCK_STREAM, 0);
	if (conn == INVALID_SOCKET) {
		WSADATA wsa;
		rc = WSAStartup(MAKEWORD(2,2), &wsa);
	}
#endif
	return rc;
}

extern int net_free(void) {
	int rc = 0;
#ifdef __WIN32
	rc = WSACleanup();
#endif
	return rc;
}

extern int net_sock(net_conn *state) {
	if (state == NULL) {
		return -1;
	}
	return state->conn;
}

extern int net_port(net_conn *state) {
	if (state == NULL) {
		return -1;
	}
	return state->port;
}

extern char *net_addr(net_conn *state) {
	if (state == NULL) {
		return NULL;
	}
	return state->addr;
}

extern net_conn *net_listen(const char *ipv4, int port) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		return NULL;
	}
#ifdef __unix__
	int opt = 1;
#elif __WIN32
	char opt = 1;
#endif
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		_close(listener);
		return NULL;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipv4);
	if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		_close(listener);
		return NULL;
	}
	if (listen(listener, SOMAXCONN) != 0) {
		_close(listener);
		return NULL;
	}
	net_conn *state = (net_conn*)malloc(sizeof(net_conn));
	state->conn = listener;
	state->port = port;
	_strncpy(state->addr, ipv4, ADRSIZ);
	return state;
}

extern net_conn *net_accept(net_conn *state) {
	if (state == NULL) {
		return NULL;
	}
	struct sockaddr_in client;
	socklen_t addrsize = sizeof(client);
	int conn = accept(state->conn, (struct sockaddr *)&client, &addrsize);
	if (conn < 0) {
		return NULL;
	}
	net_conn *nstate = (net_conn*)malloc(sizeof(net_conn));
	nstate->conn = conn;
	nstate->port = ntohs(client.sin_port);
	_strncpy(nstate->addr, inet_ntoa(client.sin_addr), ADRSIZ);
	return nstate;
}

extern int net_http_get(net_conn *state, const char *path) {
	char buffer[BUFSIZ_4K];
	snprintf(buffer, BUFSIZ_4K, 
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n\r\n", path, state->addr);
	return net_send(state, buffer, strlen(buffer));
}

extern int net_http_post(net_conn *state, const char *path, const char *data) {
	char buffer[BUFSIZ_4K];
	size_t dlen = strlen(data);
	snprintf(buffer, BUFSIZ_4K,
		"POST %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: %ld\r\n\r\n", path, state->addr, dlen);
	net_send(state, buffer, strlen(buffer));
	return net_send(state, data, dlen);
}

extern net_conn *net_socks5_connect(const char *hostname, int port, int s5port) {
	const char hostlen = strlen(hostname);
	char buffer[BUFSIZ_1K];
	net_conn *state = net_connect("127.0.0.1", s5port);
	if (state == NULL) {
		return NULL;
	}
	/* connect */
	memcpy(buffer, (char[]){5, 1, 0}, 3);
	net_send(state, buffer, 3);
	net_recv(state, buffer, BUFSIZ_1K);
	/* request */
	state->port = port;
	memcpy(buffer, (char[]){5, 1, 0, 3, hostlen}, 5);
	memcpy(buffer+5, hostname, hostlen);
	memcpy(buffer+5+hostlen, (char[]){(state->port >> 8) & 0xFF, state->port & 0xFF}, 2);
	net_send(state, buffer, 5+hostlen+2);
	net_recv(state, buffer, BUFSIZ_1K);
	if (buffer[1] != 0) {
		net_close(state);
		return NULL;
	}
	_strncpy(state->addr, hostname, ADRSIZ);
	return state;
}

extern net_conn *net_connect(const char *ipv4, int port) {
	int conn = socket(AF_INET, SOCK_STREAM, 0);
	if (conn < 0) {
		return NULL;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipv4);
	if (connect(conn, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		_close(conn);
		return NULL;
	}
	net_conn *state = (net_conn*)malloc(sizeof(net_conn));
	state->conn = conn;
	state->port = port;
	_strncpy(state->addr, ipv4, ADRSIZ);
	return state;
}

extern int net_close(net_conn *state) {
	int rc = -2;
	if (state == NULL) {
		return rc;
	}
	rc = _close(state->conn);
	free(state);
	return rc;
}

extern int net_send(net_conn *state, const char *data, int size) {
	if (state == NULL) {
		return -2;
	}
	return send(state->conn, data, size, 0);
}

extern int net_recv(net_conn *state, char *data, int size) {
	if (state == NULL) {
		return -2;
	}
	return recv(state->conn, data, size, 0);
}

static int _close(int conn) {
#ifdef __unix__
	return close(conn);
#elif __WIN32
	return closesocket(conn);
#endif
}

static char *_strncpy(char *output, const char *input, size_t size) {
	char *ret = strncpy(output, input, size-1);
	output[size-1] = '\0';
	return ret;
}
