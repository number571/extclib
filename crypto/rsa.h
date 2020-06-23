#ifndef EXTCLIB_RSA_H_
#define EXTCLIB_RSA_H_

#include <stdio.h>
#include <stdint.h>

#include "_macro_ctx.h"

typedef struct RSA RSA;

extern RSA *new_rsa(size_t size);
extern void free_rsa(RSA *keys);

extern size_t size_rsa(RSA *rsa);
extern size_t sizeof_rsa(void);

extern RSA *pemin_rsa(FILE *stream);
extern void pemout_rsa(RSA *rsa, FILE *stream, _Bool ispriv);

extern RSA *pemload_rsa(uint8_t *priv);
extern void pemstore_rsa(RSA *rsa, uint8_t *priv, uint8_t *pub);

extern void pemsize_rsa(RSA *rsa, size_t *priv, size_t *pub);

extern int8_t encrypt_rsa(Context ctx, RSA *keys);
extern int8_t decrypt_rsa(Context ctx, RSA *keys);

#endif /* EXTCLIB_RSA_H_ */
