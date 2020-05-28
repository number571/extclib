#ifndef EXTCLIB_NET_H_
#define EXTCLIB_NET_H_

#include <stddef.h>
#include <stdint.h>

extern int listen_net(uint8_t *address);
extern int accept_net(int listener);

extern int connect_net(uint8_t *address);
extern int close_net(int conn);

extern int send_net(int conn, uint8_t *buffer, size_t size);
extern int recv_net(int conn, uint8_t *buffer, size_t size);

#endif /* EXTCLIB_NET_H_ */
