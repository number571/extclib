#ifndef EXTCLIB_RSA_H_
#define EXTCLIB_RSA_H_

#include <stdio.h>
#include <stdint.h>

#include "_macro_ctx.h"

typedef struct RSA RSA_e;

extern RSA_e *new_rsa(size_t size);
extern void free_rsa(RSA_e *rsa);

extern size_t size_rsa(RSA_e *rsa);

extern RSA_e *pemin_rsa(FILE *stream);
extern void pemout_rsa(RSA_e *rsa, FILE *stream, _Bool ispriv);

extern RSA_e *pemload_rsa(uint8_t *priv);
extern void pemstore_rsa(RSA_e *rsa, uint8_t *priv, uint8_t *pub);

extern void pemsize_rsa(RSA_e *rsa, size_t *priv, size_t *pub);

extern int8_t encrypt_rsa(Context ctx, RSA_e *rsa);
extern int8_t decrypt_rsa(Context ctx, RSA_e *rsa);

#endif /* EXTCLIB_RSA_H_ */
