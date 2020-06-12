#ifndef EXTCLIB_CONTEXT_H_
#define EXTCLIB_CONTEXT_H_

#include <stddef.h>
#include <stdint.h>

typedef enum ctxmode_t {
	A_MODE_CTX,
	B_MODE_CTX,
	C_MODE_CTX,
} ctxmode_t;

typedef enum option_t {
	A_OPTION_CTX,
	B_OPTION_CTX,
} option_t;

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

// crypto.h
#define DEF_MODE A_MODE_CTX
#define ECB_MODE B_MODE_CTX
#define CBC_MODE C_MODE_CTX

#define ENCRYPT_OPTION A_OPTION_CTX
#define DECRYPT_OPTION B_OPTION_CTX

// encoding.h
#define ENCODE_OPTION A_OPTION_CTX
#define DECODE_OPTION B_OPTION_CTX

#endif /* EXTCLIB_CONTEXT_H_ */
