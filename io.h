#ifndef EXTCLIB_IO_H_
#define EXTCLIB_IO_H_

#include <stddef.h>
#include <stdint.h>

extern void printf_io(uint8_t *fmt, ...);
extern void inputs_io(uint8_t *buffer, size_t size);

#endif /* EXTCLIB_IO_H_ */
