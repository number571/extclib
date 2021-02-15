#ifdef __unix__
    #include <stdio.h>
#elif _WIN32
    #include <windows.h>
    #include <wincrypt.h>
#else
    #warning scrypto.h: used insecure entropy generator
    #include <stdlib.h>
    #include <time.h>
#endif

#include <stdint.h>

static void swap(uint8_t *x, uint8_t *y);

static uint8_t Sbox[256];

// RC4
extern void scrypto_srand(const char *key, int ksize) {
	uint8_t j = 0;
	for (int i = 0; i < 256; ++i) {
		Sbox[i] = i;
	}
	for (int i = 0; i < 256; ++i) {
		j = j + Sbox[i] + (uint8_t)key[i % ksize];
		swap(&Sbox[i], &Sbox[j]);
	}
}

// RC4
extern void scrypto_rand(char *output, int size) {
	uint8_t i = 0, j = 0;
	uint8_t t;
	for (int k = 0; k < size; ++k) {
		i += 1;
		j += Sbox[i];
		swap(&Sbox[i], &Sbox[j]);
		t = Sbox[i] + Sbox[j];
		output[k] = (char)Sbox[t];
	}
}

extern void scrypto_xor(char *output, const char *key, int ksize, const char *input, int size) {
	for (int i = 0; i < size; ++i) {
		output[i] = input[i] ^ key[i % ksize];
	}
}

extern int scrypto_entropy(char *output, int size) {
#ifdef __unix__
    FILE *file = fopen("/dev/random", "rb");
    if (file == NULL) {
        return 1;
    }
    fread(output, sizeof(char), size, file);
    fclose(file);
#elif _WIN32
    HCRYPTPROV hCryptProv;
    CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    BOOL result = CryptGenRandom(hCryptProv, sizeof(char) * size, output);
    if (result == 0) {
        return 1;
    }
    CryptReleaseContext(hCryptProv, 0);
#else
    srand(time(NULL));
    for (size_t i = 0; i < size; ++i) {
        output[i] = rand() % UINT8_MAX;
    }
#endif
    return 0;
}

static void swap(uint8_t *x, uint8_t *y) {
	uint8_t t = 0;
	t = *x;
	*x = *y;
	*y = t;
}
