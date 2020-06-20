#ifndef EXTCLIB_CRYPTO_SHA256_H_
#define EXTCLIB_CRYPTO_SHA256_H_

#include <openssl/sha.h>

#include "context.h"

static void _sum_sha256(Context *ctx) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, ctx->data.in, ctx->data.size);
    SHA256_Final(ctx->data.out, &sha256);
}

#endif /* EXTCLIB_CRYPTO_SHA256_H_ */
