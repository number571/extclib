#ifndef EXTCLIB_NET_H_
#define EXTCLIB_NET_H_

typedef struct net_t net_t;

#ifdef _WIN32
	extern int net_init(void);
	extern int net_free(void);
#endif

extern net_t *net_listen(const char *ipv4, int port);
extern net_t *net_accept(net_t *state);

extern char *net_addr(net_t *state);
extern int net_port(net_t *state);
extern int net_sock(net_t *state);

extern net_t *net_connect(const char *ipv4, int port);
extern int net_send(net_t *state, const char *data, int size);
extern int net_recv(net_t *state, char *data, int size);
extern int net_close(net_t *state);

#endif /* EXTCLIB_NET_H_ */
