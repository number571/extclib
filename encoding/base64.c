#include <openssl/evp.h>

#include "base64.h"

#include "_types.h"
#include "../context.h"

static void _base64(option_t option, Context *ctx);

extern void encode_base64(Context ctx) {
	_base64(ENCODE_OPTION, &ctx);
}

extern void decode_base64(Context ctx) {
	_base64(DECODE_OPTION, &ctx);
}

static void _base64(option_t option, Context *ctx) {
	switch(option) {
		case ENCODE_OPTION:
			EVP_EncodeBlock(ctx->data.out, ctx->data.in, ctx->data.size);
		break;
		case DECODE_OPTION:
			EVP_DecodeBlock(ctx->data.out, ctx->data.in, ctx->data.size);
		break;
	}
}
