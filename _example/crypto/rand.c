#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extclib/crypto.h"

#if __WIN32
    #include <windows.h>
    #include <wincrypt.h>
#endif

#define SEEDSIZE 32
#define BUFFSIZE 20

void print_bytes(uint8_t * array, size_t length);

void init(void) {
    uint8_t seed[SEEDSIZE] = {0};

#ifdef __linux__

    FILE *file = fopen("/dev/random", "rb");
    if (file == NULL) {
        fprintf(stderr, "%s\n", "failed open: '/dev/random'");
        exit(1);
    }
    fread(seed, sizeof(uint8_t), SEEDSIZE, file);
    fclose(file);

#elif __WIN32

    HCRYPTPROV hCryptProv;
    CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    BOOL result = CryptGenRandom(hCryptProv, sizeof(uint8_t) * SEEDSIZE, seed);
    if (result == 0) {
        fprintf(stderr, "%s\n", "failed call: 'CryptGenRandom'");
        exit(1);
    }
    CryptReleaseContext(hCryptProv, 0);

#endif
    
    srand_crypto((Context){
        .data = {
            .size = SEEDSIZE,
            .in = seed,
        },
    });
}

int main(void) {
    init(); // call srand
    uint8_t buffer[BUFFSIZE] = {0};
    Context ctx = {
        .data = {
            .size = BUFFSIZE,
            .out = buffer,
        },
    };
    for (size_t i = 0; i < 10; ++i) {
        rand_crypto(ctx);
        print_bytes(buffer, BUFFSIZE);
    }
    return 0;
}

void print_bytes(uint8_t * array, size_t length) {
    printf("[ ");
    for (size_t i = 0; i < length; ++i) {
        if (array[i] <= 0xF) {
            putchar('0');
        }
        printf("%x ", array[i]);
    }
    printf("]\n");
}
