#ifndef EXTCLIB_CONTEXT_H_
#define EXTCLIB_CONTEXT_H_

#include <stddef.h>
#include <stdint.h>

typedef enum ctxmode_t {
	A_MODE_CTX,
	B_MODE_CTX,
	C_MODE_CTX,
    D_MODE_CTX
} ctxmode_t;

typedef struct Context {
    ctxmode_t mode;
    struct {
        size_t  size;
        uint8_t  *in;
        uint8_t *out;
    } data;
    struct {
        size_t   size;
        uint8_t *vec1;
        uint8_t *vec2;
    } desc;
} Context;

#endif /* EXTCLIB_CONTEXT_H_ */
