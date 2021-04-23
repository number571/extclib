#ifdef __unix__
	#include <unistd.h>
	#include <arpa/inet.h>
#elif _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
#else
	#warning "net.h: platform not supported"
#endif

#include "net.h"

#include <stdlib.h>
#include <string.h>

#define ADRSIZ 256

typedef struct net_t {
	int  conn;
	int  port;
	char addr[ADRSIZ];
} net_t;

static int _close(int conn);
static char *_strncpy(char *output, const char *input, size_t size);

#ifdef _WIN32
	extern int net_init(void) {
		int rc = 0;
		int conn = socket(AF_INET, SOCK_STREAM, 0);
		if (conn == INVALID_SOCKET) {
			WSADATA wsa;
			rc = WSAStartup(MAKEWORD(2,2), &wsa);
		}
		return rc;
	}
	extern int net_free(void) {
		return WSACleanup();
	}
#endif

extern int net_sock(net_t *state) {
	if (state == NULL) {
		return -1;
	}
	return state->conn;
}

extern int net_port(net_t *state) {
	if (state == NULL) {
		return -1;
	}
	return state->port;
}

extern char *net_addr(net_t *state) {
	if (state == NULL) {
		return NULL;
	}
	return state->addr;
}

extern net_t *net_listen(const char *ipv4, int port) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		return NULL;
	}
#ifdef __unix__
	int opt = 1;
#elif _WIN32
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
	net_t *state = (net_t*)malloc(sizeof(net_t));
	state->conn = listener;
	state->port = port;
	_strncpy(state->addr, ipv4, ADRSIZ);
	return state;
}

extern net_t *net_accept(net_t *state) {
	if (state == NULL) {
		return NULL;
	}
	struct sockaddr_in client;
	socklen_t addrsize = sizeof(client);
	int conn = accept(state->conn, (struct sockaddr *)&client, &addrsize);
	if (conn < 0) {
		return NULL;
	}
	net_t *nstate = (net_t*)malloc(sizeof(net_t));
	nstate->conn = conn;
	nstate->port = ntohs(client.sin_port);
	_strncpy(nstate->addr, inet_ntoa(client.sin_addr), ADRSIZ);
	return nstate;
}

extern net_t *net_connect(const char *ipv4, int port) {
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
	net_t *state = (net_t*)malloc(sizeof(net_t));
	state->conn = conn;
	state->port = port;
	_strncpy(state->addr, ipv4, ADRSIZ);
	return state;
}

extern int net_close(net_t *state) {
	int rc = -2;
	if (state == NULL) {
		return rc;
	}
	rc = _close(state->conn);
	free(state);
	return rc;
}

extern int net_send(net_t *state, const char *data, int size) {
	if (state == NULL) {
		return -2;
	}
	return send(state->conn, data, size, 0);
}

extern int net_recv(net_t *state, char *data, int size) {
	if (state == NULL) {
		return -2;
	}
	return recv(state->conn, data, size, 0);
}

static int _close(int conn) {
#ifdef __unix__
	return close(conn);
#elif _WIN32
	return closesocket(conn);
#endif
}

static char *_strncpy(char *output, const char *input, size_t size) {
	char *ret = strncpy(output, input, size-1);
	output[size-1] = '\0';
	return ret;
}
