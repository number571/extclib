#ifndef EXTCLIB_RSA_H_
#define EXTCLIB_RSA_H_

#include <stdint.h>

#include "context.h"

typedef struct KeysRSA KeysRSA;

extern KeysRSA *new_rsa(size_t size);
extern void free_rsa(KeysRSA *keys);

extern size_t size_rsa(KeysRSA *rsa);
extern size_t sizeof_rsa(void);

extern void pemsize_rsa(KeysRSA *rsa, size_t *priv, size_t *pub);
extern void pemstore_rsa(KeysRSA *rsa, uint8_t *priv, uint8_t *pub);
extern KeysRSA *pemload_rsa(uint8_t *priv);

extern int8_t encrypt_rsa(Context ctx, KeysRSA *keys);
extern int8_t decrypt_rsa(Context ctx, KeysRSA *keys);

#endif /* EXTCLIB_RSA_H_ */
