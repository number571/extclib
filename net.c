#ifdef __unix__
	#include <unistd.h>
	#include <arpa/inet.h>
#elif _WIN32
	#ifdef _MSC_VER
		#pragma comment(lib, "ws2_32.lib")
	#endif
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
	int  sock;
	int  port;
	char addr[ADRSIZ];
} net_t;

static int _close(int sock);
static char *_strncpy(char *output, const char *input, size_t size);

#ifdef _WIN32
	extern int net_init(void) {
		int rc, sock;
		rc = 0;

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			WSADATA wsa;
			rc = WSAStartup(MAKEWORD(2,2), &wsa);
		}

		_close(sock);
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
	return state->sock;
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
	int sock;
	struct sockaddr_in addr;
	net_t *state;
#ifdef __unix__
	int opt;
#elif _WIN32
	char opt;
#endif

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return NULL;
	}

	opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		_close(sock);
		return NULL;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipv4);
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		_close(sock);
		return NULL;
	}

	if (listen(sock, SOMAXCONN) != 0) {
		_close(sock);
		return NULL;
	}

	state = (net_t*)malloc(sizeof(net_t));
	state->sock = sock;
	state->port = port;
	_strncpy(state->addr, ipv4, ADRSIZ);

	return state;
}

extern net_t *net_accept(net_t *state) {
	struct sockaddr_in client;
	socklen_t addrsize;
	int sock;
	net_t *nstate;

	if (state == NULL) {
		return NULL;
	}

	addrsize = sizeof(client);
	sock = accept(state->sock, (struct sockaddr *)&client, &addrsize);
	if (sock < 0) {
		return NULL;
	}

	nstate = (net_t*)malloc(sizeof(net_t));
	nstate->sock = sock;
	nstate->port = ntohs(client.sin_port);
	_strncpy(nstate->addr, inet_ntoa(client.sin_addr), ADRSIZ);

	return nstate;
}

extern net_t *net_connect(const char *ipv4, int port) {
	int sock;
	struct sockaddr_in addr;
	net_t *state;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return NULL;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipv4);
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		_close(sock);
		return NULL;
	}

	state = (net_t*)malloc(sizeof(net_t));
	state->sock = sock;
	state->port = port;
	_strncpy(state->addr, ipv4, ADRSIZ);

	return state;
}

extern int net_close(net_t *state) {
	int rc;
	rc = -2;

	if (state == NULL) {
		return rc;
	}

	rc = _close(state->sock);
	free(state);

	return rc;
}

extern int net_send(net_t *state, const char *data, int size) {
	if (state == NULL) {
		return -2;
	}
	return send(state->sock, data, size, 0);
}

extern int net_recv(net_t *state, char *data, int size) {
	if (state == NULL) {
		return -2;
	}
	return recv(state->sock, data, size, 0);
}

static int _close(int sock) {
#ifdef __unix__
	return close(sock);
#elif _WIN32
	return closesocket(sock);
#endif
}

static char *_strncpy(char *output, const char *input, size_t size) {
	char *ret;
	
	ret = strncpy(output, input, size-1);
	output[size-1] = '\0';

	return ret;
}
