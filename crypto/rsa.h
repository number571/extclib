#ifndef EXTCLIB_RSA_H_
#define EXTCLIB_RSA_H_

#include <stdint.h>

#include "context.h"

typedef struct KeysRSA KeysRSA;

extern KeysRSA *new_rsa(size_t size);
extern void free_rsa(KeysRSA *keys);

extern int8_t encrypt_rsa(Context ctx, KeysRSA *keys);
extern int8_t decrypt_rsa(Context ctx, KeysRSA *keys);

#endif /* EXTCLIB_RSA_H_ */
