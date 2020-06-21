#include <openssl/evp.h>

#include "../context.h"
#include "base64.h"

extern void encode_base64(Context ctx) {
	EVP_EncodeBlock(ctx.data.out, ctx.data.in, ctx.data.size);
}

extern void decode_base64(Context ctx) {
	EVP_DecodeBlock(ctx.data.out, ctx.data.in, ctx.data.size);
}
