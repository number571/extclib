#ifndef EXTCLIB_AES_H_
#define EXTCLIB_AES_H_

#include <stdint.h>

#include "context.h"

extern int8_t encrypt_aes(Context ctx);
extern int8_t decrypt_aes(Context ctx);

#endif /* EXTCLIB_AES_H_ */
