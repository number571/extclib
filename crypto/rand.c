#include <stdint.h>

#include <openssl/rand.h>

#include "rand.h"
#include "context.h"

extern int8_t bytes_rand(Context ctx) {
    return !(RAND_bytes(ctx.data.out, ctx.data.size) == 1);
}
