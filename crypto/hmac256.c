#include <stdint.h>
#include <string.h>

#include <openssl/sha.h>

#include "hmac256.h"
#include "../context.h"

#include "_sha256.h"

extern void sum_hmac256(Context ctx) {
    const uint8_t hsize = 32; // sha256 size
    const uint8_t bsize = 64; // hmac block size RFC 2104
    const uint8_t ipad = 0x36;
    const uint8_t opad = 0x5c;

    Context temp_ctx;
    uint8_t keybuff[bsize];
    uint8_t ikeypad[bsize];
    uint8_t okeypad[bsize];
    uint8_t result[ctx.data.size+bsize+hsize];

    if (ctx.desc.size > bsize) {
        temp_ctx.data.size = ctx.desc.size;
        temp_ctx.data.in = ctx.desc.vec1;
        temp_ctx.data.out = keybuff;
        _sum_sha256(&temp_ctx);
    }
    if (ctx.desc.size < bsize) {
        memcpy(keybuff, ctx.desc.vec1, ctx.desc.size);
        memset(keybuff + ctx.desc.size, 0, bsize - ctx.desc.size);
    }

    for (uint8_t i = 0; i < bsize; ++i) {
        ikeypad[i] = keybuff[i] ^ ipad;
        okeypad[i] = keybuff[i] ^ opad;
    }

    memcpy(result, ikeypad, bsize);
    memcpy(result + bsize, ctx.data.in, ctx.data.size);
    temp_ctx.data.size = ctx.data.size + bsize;
    temp_ctx.data.in = result;
    temp_ctx.data.out = keybuff;
    _sum_sha256(&temp_ctx);

    memcpy(result, okeypad, bsize);
    memcpy(result + bsize, keybuff, hsize);
    temp_ctx.data.size = bsize + hsize;
    temp_ctx.data.in = result;
    temp_ctx.data.out = ctx.data.out;
    _sum_sha256(&temp_ctx);
}
