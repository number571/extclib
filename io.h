#ifndef EXTCLIB_IO_H_
#define EXTCLIB_IO_H_

#include <stddef.h>
#include <stdint.h>

extern void fmt_print(uint8_t *fmt, ...);
extern void fmt_println(uint8_t *fmt, ...);

extern void input_string(uint8_t *buffer, size_t size);

#endif /* EXTCLIB_IO_H_ */
