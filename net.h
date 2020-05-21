#pragma once

#include <stddef.h>
#include <stdint.h>

extern int tcp_listen(uint8_t *address);
extern int tcp_connect(uint8_t *address);
extern int tcp_accept(int listener);
extern int tcp_send(int conn, uint8_t *buffer, size_t size);
extern int tcp_recv(int conn, uint8_t *buffer, size_t size);
extern int tcp_close(int conn);
