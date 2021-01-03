#ifndef EXTCLIB_NET_H
#define EXTCLIB_NET_H

typedef struct net_conn net_conn;

extern net_conn *net_socks5_connect(const char *hostname, int port, int s5port);
extern int net_http_get(net_conn *state, const char *path);
extern int net_http_post(net_conn *state, const char *path, const char *data);

extern int net_init(void);
extern int net_free(void);

extern net_conn *net_listen(const char *ipv4, int port);
extern net_conn *net_accept(net_conn *state);

extern char *net_addr(net_conn *state);
extern int net_port(net_conn *state);
extern int net_sock(net_conn *state);

extern net_conn *net_connect(const char *ipv4, int port);
extern int net_send(net_conn *state, const char *data, int size);
extern int net_recv(net_conn *state, char *data, int size);
extern int net_close(net_conn *state);

#endif /* EXTCLIB_NET_H */
