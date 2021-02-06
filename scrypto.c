#include <stdint.h>

static void swap(uint8_t *x, uint8_t *y);

static uint8_t Sbox[256];

extern void scrypto_rc4_init(char *key, int ksize) {
	uint8_t j = 0;
	for (int i = 0; i < 256; ++i) {
		Sbox[i] = i;
	}
	for (int i = 0; i < 256; ++i) {
		j = j + Sbox[i] + (uint8_t)key[i % ksize];
		swap(&Sbox[i], &Sbox[j]);
	}
}

extern void scrypto_rc4_generate(char *output, int size) {
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

extern void scrypto_xor(char *output, char *key, int ksize, char *input, int size) {
	for (int i = 0; i < size; ++i) {
		output[i] = input[i] ^ key[i % ksize];
	}
}

static void swap(uint8_t *x, uint8_t *y) {
	uint8_t t = 0;
	t = *x;
	*x = *y;
	*y = t;
}
