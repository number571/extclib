#include "akey.h"
#include "../crypto.h"
#include "../type/bigint.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct akey_t {
	int size;    // size in bytes
	bigint_t *y; // public value
	bigint_t *x; // private value
	bigint_t *g; // generator
	bigint_t *p; // module
} akey_t;

static void _generate_rand_bigint(bigint_t *num, int bits);
static bigint_t *_generate_prime(int bits);

extern akey_t *akey_new(int bytes) {
	const int BITSIZE = bytes*8;

	bigint_t *t1, *t2;
	akey_t *key;

	key = (akey_t*)malloc(sizeof(akey_t));
	key->size = bytes;

	// create module
	key->p = _generate_prime(BITSIZE);
	key->g = bigint_new("0");
	key->x = bigint_new("0");
	key->y = bigint_new("0");

	t1 = bigint_new("0");
	t2 = bigint_new("0");

	// t1 = p-1
	bigint_sub_ui(t1, key->p, 1);

	// create generator
	while(1) {
		_generate_rand_bigint(key->g, BITSIZE);
		// should be: 1 < g < p-1
		if ((bigint_cmp_ui(key->g, 1) <= 0) || (bigint_cmp(key->g, t1) >= 0)) {
			continue;
		}
		break;
	};

	// create session value
	while(1) {
		_generate_rand_bigint(key->x, BITSIZE);
		// should be: 1 < x < p-1
		if ((bigint_cmp_ui(key->x, 1) <= 0) || (bigint_cmp(key->x, t1) >= 0)) {
			continue;
		}
		// should be: gcd(x, p-1) = 1
		bigint_gcd(t2, key->x, t1);
		if (bigint_cmp_ui(t2, 1) != 0) {
			continue;
		}
		break;
	}

	// create public value
	bigint_expmod(key->y, key->g, key->x, key->p);

	bigint_free(t1);
	bigint_free(t2);

	return key;
}

extern void akey_free(akey_t *key) {
	bigint_free(key->p);
	bigint_free(key->g);
	bigint_free(key->x);
	bigint_free(key->y);
	free(key);
}

extern int akey_size(akey_t *key) {
	return key->size;
}

extern akey_t *akey_set(
	int bytes, 
	int ktype,
	bigint_t *k,
	bigint_t *g, 
	bigint_t *p
) {
	bigint_t *t1;
	akey_t *key;

	key = NULL;
	t1 = bigint_new("0");

	if (k == NULL) {
		goto close;
	}

	if (ktype != AKEY_PUBLIC && ktype != AKEY_PRIVATE) {
		goto close;
	}

	if (!bigint_isprime(p)){
		goto close;
	}

	// t1 = p-1
	bigint_sub_ui(t1, p, 1);

	// should be: 1 < g < p-1
	if ((bigint_cmp_ui(g, 1) <= 0) || (bigint_cmp(g, t1) >= 0)) {
		goto close;
	}

	if (ktype == AKEY_PRIVATE) {
		// should be: 1 < x < p-1
		if ((bigint_cmp_ui(k, 1) <= 0) || (bigint_cmp(k, t1) >= 0)) {
			goto close;
		}
		// should be: gcd(x, p-1) = 1
		bigint_gcd(t1, k, t1);
		if (bigint_cmp_ui(t1, 1) != 0) {
			goto close;
		}
	}

	key = (akey_t*)malloc(sizeof(akey_t));
	key->size = bytes;

	key->p = bigint_new("0");
	key->g = bigint_new("0");
	key->x = bigint_new("0");
	key->y = bigint_new("0");

	bigint_cpy(key->p, p);
	bigint_cpy(key->g, g);

	switch(ktype) {
		case AKEY_PUBLIC: {
			bigint_cpy(key->y, k);
		}
		break;
		case AKEY_PRIVATE: {
			bigint_cpy(key->x, k);
			bigint_expmod(key->y, g, k, p);
		}
		break;
	}

close:

	bigint_free(t1);

	return key;
}

extern bigint_t *akey_p(akey_t *key) {
	return key->p;
}

extern bigint_t *akey_g(akey_t *key) {
	return key->g;
}

extern bigint_t *akey_x(akey_t *key) {
	return key->x;
}

extern bigint_t *akey_y(akey_t *key) {
	return key->y;
}

extern int akey_encrypt (
	unsigned char * output,
	const akey_t *key,
	const unsigned char * const input
) {
	const int BITSIZE = key->size*8;

	bigint_t *t1, *t2;
	bigint_t *k, *a, *b;

	t1 = bigint_new("0");
	t2 = bigint_new("0");

	// session key
	k = bigint_new("0");

	// t1 = p-1
	bigint_sub_ui(t1, key->p, 1);

	// (a, b) - encrypt message
	a = bigint_new("0");
	b = bigint_new("0");

	// create session value
	while(1) {
		_generate_rand_bigint(k, BITSIZE);
		// should be: 1 < k < p-1
		if ((bigint_cmp_ui(k, 1) <= 0) || (bigint_cmp(k, t1) >= 0)) {
			continue;
		}
		// should be: gcd(k, p-1) = 1
		bigint_gcd(t2, k, t1);
		if (bigint_cmp_ui(t2, 1) != 0) {
			continue;
		}
		break;
	}

	// a = (g^k) mod p
	bigint_expmod(a, key->g, k, key->p);

	// b = (y^k * m) mod p
	bigint_load(t2, input, key->size);
	bigint_expmod(b, key->y, k, key->p);
	bigint_mul(b, b, t2);
	bigint_mod(b, b, key->p);

	memset(output, 0, key->size*2);

	bigint_save(output, a, key->size);
	bigint_save(output+(key->size), b, key->size);

	bigint_free(a);
	bigint_free(b);

	bigint_free(k);

	bigint_free(t1);
	bigint_free(t2);

	return 0;
}

extern int akey_decrypt (
	unsigned char * output,
	const akey_t *key,
	const unsigned char * const input
) {
	bigint_t *t1;
	bigint_t *a, *b;

	if (bigint_cmp_ui(key->x, 0) == 0) {
		return 1;
	}

	t1 = bigint_new("0");

	a = bigint_new("0");
	b = bigint_new("0");

	bigint_load(a, input, key->size);
	bigint_load(b, input+(key->size), key->size);

	// m = (b * a^(p-1-x)) mod p
	bigint_sub_ui(t1, key->p, 1);
	bigint_sub(t1, t1, key->x);
	bigint_expmod(a, a, t1, key->p);
	bigint_mul(t1, b, a);
	bigint_mod(t1, t1, key->p);

	memset(output, 0, key->size);
	bigint_save(output, t1, key->size);

	bigint_free(a);
	bigint_free(b);

	bigint_free(t1);

	return 0;
}

static void _generate_rand_bigint(bigint_t *num, int bits) {
	const int BSIZE = bits/8;

	uint8_t buffer[BSIZE];

	crypto_rand(buffer, BSIZE);
	bigint_load(num, buffer, BSIZE);
}

static bigint_t *_generate_prime(int bits) {
	bigint_t *num, *tmp, *max, *min;

	num = bigint_new("1");
	tmp = bigint_new("0");

	max = bigint_new("1");
	min = bigint_new("1");

	bigint_shl(max, max, bits);
	bigint_shl(min, min, bits-1);

	while(1) {
	generate_new:
		_generate_rand_bigint(num, bits);
		bigint_add(num, num, min);
		bigint_mod_ui(tmp, num, 2);
		if (bigint_cmp_ui(tmp, 0) == 0) {
			bigint_add_ui(num, num, 1);
		}
		do {
			bigint_add_ui(num, num, 2);
			if (bigint_cmp(num, max) >= 0) {
				goto generate_new;
			}
		} while(!bigint_isprime(num));
		break;
	}

	bigint_free(min);
	bigint_free(max);
	bigint_free(tmp);

	return num;
}
