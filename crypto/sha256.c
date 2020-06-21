#include "sha256.h"
#include "../context.h"

#include "_sha256.h"

extern void sum_sha256(Context ctx) {
    _sum_sha256(&ctx);
}


