#ifdef __unix__
	#include <stdio.h>
#elif _WIN32
	#include <windows.h>
	#include <wincrypt.h>
#else
	#warning crypto.h: used insecure entropy generator
	#include <stdlib.h>
	#include <time.h>
#endif

#include "crypto.h"

#include <stdint.h>
#include <string.h>

static void _split_64bits_to_32bits(uint64_t b64, uint32_t * b32_1, uint32_t * b32_2);
static uint64_t _join_32bits_to_64bits(uint32_t b32_1, uint32_t b32_2);

static inline int _hex(uint8_t ch);
static inline void _swap(uint8_t *x, uint8_t *y);

extern int crypto_entropy(unsigned char *output, int size) {
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
		output[i] = rand();
	}
#endif
	return 0;
}

/* BEGIN: RC4 */
static uint8_t sbox_rc4[256];

extern void crypto_srand(const unsigned char *key, int ksize) {
	uint8_t j = 0;
	for (int i = 0; i < 256; ++i) {
		sbox_rc4[i] = i;
	}
	for (int i = 0; i < 256; ++i) {
		j = j + sbox_rc4[i] + (uint8_t)key[i % ksize];
		_swap(&sbox_rc4[i], &sbox_rc4[j]);
	}
}

extern void crypto_rand(unsigned char *output, int size) {
	uint8_t i = 0, j = 0;
	uint8_t t;
	for (int k = 0; k < size; ++k) {
		i += 1;
		j += sbox_rc4[i];
		_swap(&sbox_rc4[i], &sbox_rc4[j]);
		t = sbox_rc4[i] + sbox_rc4[j];
		output[k] = sbox_rc4[t];
	}
}
/* END: RC4 */

/* BEGIN: XTEA */
extern unsigned long long crypto_xtea(
	int mode, 
	unsigned long long data, 
	const unsigned long key[4]
) {
	const int D = 0x9E3779B9;
	const int N = 32;
	uint32_t v[2], sum;
	_split_64bits_to_32bits(data, &v[0], &v[1]);
	switch(mode) {
		case CRYPTO_MODE_ENCRYPT:
			sum = 0;
			for (int i = 0; i < N; ++i) {
				v[0]  += (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + key[sum & 3]);
				sum += D;
				v[1]  += (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + key[(sum>>11) & 3]);
			}
		break;
		case CRYPTO_MODE_DECRYPT:
			sum = D*N;
			for (int i = 0; i < N; ++i) {
				v[1]  -= (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + key[(sum>>11) & 3]);
				sum -= D;
				v[0]  -= (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + key[sum & 3]);
			}
		break;
	}
	return _join_32bits_to_64bits(v[0], v[1]);
}
/* END: XTEA */

extern int crypto_hex(
	int mode,
	unsigned char *output,
	const unsigned char *input,
	int size
) {
	const char *alpha = "0123456789ABCDEF";
	switch(mode) {
		case CRYPTO_MODE_ENCRYPT: {
			for (int i = 0; i < size; ++i) {
				output[i*2]   = alpha[input[i] >> 4];
				output[i*2+1] = alpha[input[i] & 15];
			}
			output[size*2] = '\0'; 
		}
		break;
		case CRYPTO_MODE_DECRYPT: {
			uint8_t hex1, hex2;
			size = size / 2;
			for (int i = 0; i < size; ++i) {
				hex1 = _hex(input[i*2]);
				hex2 = _hex(input[i*2+1]);
				if (hex1 == -1 || hex2 == -1) {
					return 1;
				}
				output[i] = (hex1 << 4) | hex2;
			}
			output[size] = '\0';
		}
		break;
	}
	return 0;
}

static void _split_64bits_to_32bits(uint64_t b64, uint32_t * b32_1, uint32_t * b32_2) {
    *b32_1 = (uint32_t)(b64 >> 32);
    *b32_2 = (uint32_t)(b64);
}

static uint64_t _join_32bits_to_64bits(uint32_t b32_1, uint32_t b32_2) {
    uint64_t b64;
    b64 = (uint64_t)b32_1;
    b64 = (uint64_t)(b64 << 32) | b32_2;
    return b64;
}

static inline int _hex(uint8_t ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	} else if (ch >= 'A' && ch <= 'Z') {
		return ch - 'A' + 10;
	} else if (ch >= 'a' && ch <= 'a') {
		return ch - 'a' + 10;
	}
	return -1;
} 

static inline void _swap(uint8_t *x, uint8_t *y) {
	uint8_t t = 0;
	t = *x;
	*x = *y;
	*y = t;
}
