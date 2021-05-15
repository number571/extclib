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
	const akey_t *key,
	unsigned char * output,
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

	bigint_save(a, output, key->size);
	bigint_save(b, output+(key->size), key->size);

	bigint_free(a);
	bigint_free(b);

	bigint_free(k);

	bigint_free(t1);
	bigint_free(t2);

	return 0;
}

extern int akey_decrypt (
	const akey_t *key,
	unsigned char * output,
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
	bigint_save(t1, output, key->size);

	bigint_free(a);
	bigint_free(b);

	bigint_free(t1);

	return 0;
}

extern int akey_sign (
	const akey_t *key,
	unsigned char * output,
	const unsigned char * const input,
	int isize
) {
	const int BITSIZE = key->size*8;
	const int HSIZE = 32;

	bigint_t *t1, *t2;
	bigint_t *k, *r, *s;
	uint8_t hash[HSIZE];

	if (bigint_cmp_ui(key->x, 0) == 0) {
		return 1;
	}

	t1 = bigint_new("0");
	t2 = bigint_new("0");

	// session key
	k = bigint_new("0");

	// t1 = p-1
	bigint_sub_ui(t1, key->p, 1);

	// (r, s) - signed message
	r = bigint_new("0");
	s = bigint_new("0");

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

	// r = g^k (mod p)
	bigint_expmod(r, key->g, k, key->p);

	// s = (m - xr) * k^(-1) (mod p-1)
	crypto_hash(hash, input, isize);
	bigint_load(t2, hash, HSIZE);

	bigint_mul(s, key->x, r);
	bigint_mod(s, s, t1);

	bigint_sub(s, t2, s);
	bigint_inv(t2, k, t1);

	bigint_mul(s, s, t2);
	bigint_mod(s, s, t1);

	memset(output, 0, key->size*2);

	bigint_save(r, output, key->size);
	bigint_save(s, output+(key->size), key->size);

	bigint_free(r);
	bigint_free(s);

	bigint_free(k);

	bigint_free(t1);
	bigint_free(t2);

	return 0;
}

extern int akey_verify (
	const akey_t *key,
	const unsigned char * const sign,
	const unsigned char * const input,
	int isize
) {
	const int HSIZE = 32;

	bigint_t *t1, *t2;
	bigint_t *r, *s;
	uint8_t hash[HSIZE];
	int retcode;

	t1 = bigint_new("0");
	t2 = bigint_new("0");

	r = bigint_new("0");
	s = bigint_new("0");

	bigint_load(r, sign, key->size);
	bigint_load(s, sign+(key->size), key->size);

	// t1 = p-1
	bigint_sub_ui(t1, key->p, 1);

	// should be: 0 < r < p
	if ((bigint_cmp_ui(r, 0) <= 0) || (bigint_cmp(r, key->p) >= 0)) {
		retcode = 2;
		goto close;
	}

	// should be: 0 < s < p-1
	if ((bigint_cmp_ui(r, 0) <= 0) || (bigint_cmp(r, t1) >= 0)) {
		retcode = 2;
		goto close;
	}

	// w = (y^r) * (r^s) (mod p)
	bigint_expmod(t1, key->y, r, key->p);
	bigint_expmod(t2, r, s, key->p);
	bigint_mul(t1, t1, t2);
	bigint_mod(t1, t1, key->p);

	// u = g^m (mod p)
	crypto_hash(hash, input, isize);
	bigint_load(t2, hash, HSIZE);
	bigint_expmod(t2, key->g, t2, key->p);

	if (bigint_cmp(t1, t2) == 0) {
		retcode = 0;
	} else {
		retcode = 1;
	}

close:

	bigint_free(r);
	bigint_free(s);

	bigint_free(t1);
	bigint_free(t2);

	return retcode;
}

static void _generate_rand_bigint(bigint_t *num, int bits) {
	const int BSIZE = bits/8;
	uint8_t buffer[BSIZE];

	crypto_rand(buffer, BSIZE);
	bigint_load(num, buffer, BSIZE);
}

static bigint_t *_generate_prime(int bits) {
	bigint_t *num, *tmp;
	bigint_t *max, *min;

	num = bigint_new("1");
	tmp = bigint_new("0");

	max = bigint_new("1");
	min = bigint_new("1");

	bigint_shl(max, max, bits);
	bigint_shl(min, min, bits-1);

generate_new:
	_generate_rand_bigint(num, bits);

	// RAND mod (MAX-MIN) + MIN
	bigint_sub(tmp, max, min);
	bigint_mod(num, num, tmp);
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

	bigint_free(min);
	bigint_free(max);
	bigint_free(tmp);

	return num;
}
