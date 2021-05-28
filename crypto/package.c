#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "akey.h"
#include "proof.h"
#include "package.h"
#include "../crypto.h"
#include "../type/bigint.h"

#define AKEYSIZ PACKAGE_AKEYSIZ
#define HASHSIZ 32
#define SKEYSIZ 16
#define IVECSIZ 16

// package(asymmetric, symmetric, hash, sign, proof):
// @ asymmetric = elgamal-N;
// @ symmetric  = speck-128-OFB;
// @ hash       = speck-MDC-2;
// @ sign       = elgamal-N;
// @ proof      = proof-of-work;
typedef struct package_t {
	struct {
		uint8_t title[IVECSIZ+32];   		// 256 bit title info
		uint8_t rands[IVECSIZ+32];   		// 256 bit salt
		uint8_t sesnk[AKEYSIZ*2];        	// 2N bit session key -> (a, b)
		uint8_t sendr[IVECSIZ+(AKEYSIZ*3)]; // 3N bit -> (p, g, y)
	} head;
	struct {
		uint8_t  data[IVECSIZ+512];         // 512 byte main info
		uint8_t  hash[32];         			// 256 bit hash(rands, sendr, receiver, title, data)
		uint64_t proof;            			// 64 bit proof
		uint8_t  sign[IVECSIZ+(AKEYSIZ*2)]; // 2N bit sign(hash) -> (r, s)
	} body;
	struct {
		akey_t *sendr;
	} temp;
} package_t;

static void _store_public_key(akey_t *pubkey, uint8_t *output, int ksize);
static akey_t *_load_public_key(uint8_t *input, int ksize);

extern package_t *package_new(unsigned char title[32], unsigned char data[512]) {
	package_t *pack = (package_t*)malloc(sizeof(package_t));
	memcpy(pack->head.title+IVECSIZ, title, 32);
	memcpy(pack->body.data+IVECSIZ, data, 512);
	return pack;
}

extern void package_free(package_t *pack) {
	akey_free(pack->temp.sendr);
	free(pack);
}

extern int package_size(void) {
	return sizeof(package_t);
}

extern void *package_encode(package_t *pack) {
	return (void*)pack;
}

extern package_t *package_decode(void *pack) {
	return (package_t*)pack;
}

extern int package_encrypt(package_t *pack, akey_t *sender, akey_t *receiver, int diff) {
	uint8_t tmparr[AKEYSIZ*3];
	uint8_t hashes[5][HASHSIZ];

	if (akey_size(sender) != AKEYSIZ || akey_size(receiver) != AKEYSIZ) {
		return -1;
	}

	// store public key of sender in package
	_store_public_key(sender, pack->head.sendr+IVECSIZ, AKEYSIZ);

	// store public key of receiver in local array for hash
	_store_public_key(receiver, tmparr, AKEYSIZ);

	// store random values in head.rands, head.sesnk
	memset(pack->head.sesnk, 0, AKEYSIZ*2);
	crypto_rand(pack->head.sesnk, 16);

	crypto_rand(pack->head.rands+IVECSIZ, 32);

	// generate hash
	crypto_hash(hashes[0], pack->head.rands+IVECSIZ, 32);
	crypto_hash(hashes[1], pack->head.sendr+IVECSIZ, AKEYSIZ*3);
	crypto_hash(hashes[2], tmparr, AKEYSIZ*3);
	crypto_hash(hashes[3], pack->head.title+IVECSIZ, 32);
	crypto_hash(hashes[4], pack->body.data+IVECSIZ, 512);

	memset(pack->body.hash, 0, HASHSIZ);
	for (int i = 0; i < 5; ++i) {
		memcpy(tmparr, pack->body.hash, HASHSIZ);
		memcpy(tmparr+HASHSIZ, hashes[i], HASHSIZ);
		crypto_hash(pack->body.hash, tmparr, HASHSIZ*2);
	}

	// sign hash
	if (akey_sign(sender, pack->body.sign+IVECSIZ, pack->body.hash, HASHSIZ) != 0) {
		return -2;
	}

	// encrypt head.title, head.rands, head.sendr
	crypto_rand(pack->head.title, IVECSIZ);
	crypto_encrypt(pack->head.title+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->head.title, IVECSIZ, 
		pack->head.title+IVECSIZ, 32);

	crypto_rand(pack->head.rands, IVECSIZ);
	crypto_encrypt(pack->head.rands+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->head.rands, IVECSIZ, 
		pack->head.rands+IVECSIZ, 32);

	crypto_rand(pack->head.sendr, IVECSIZ);
	crypto_encrypt(pack->head.sendr+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->head.sendr, IVECSIZ, 
		pack->head.sendr+IVECSIZ, AKEYSIZ*3);

	// encrypt body.data, body.sign
	crypto_rand(pack->body.data, IVECSIZ);
	crypto_encrypt(pack->body.data+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->body.data,  IVECSIZ, 
		pack->body.data+IVECSIZ, 512);

	crypto_rand(pack->body.sign, IVECSIZ);
	crypto_encrypt(pack->body.sign+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->body.sign,  IVECSIZ, 
		pack->body.sign+IVECSIZ, AKEYSIZ*2);

	// encrypt session key
	if (akey_encrypt(receiver, pack->head.sesnk, pack->head.sesnk) != 0) {
		return -3;
	}

	// proof hash
	pack->body.proof = proof_of_work(pack->body.hash, diff);

	return 0;
}

extern int package_decrypt(package_t *pack, akey_t *receiver, int diff) {
	akey_t *pubkey;
	int retcode;
	uint8_t tmparr[AKEYSIZ*3];
	uint8_t hashes[5][HASHSIZ];
	uint8_t checkhash[HASHSIZ];

	if (akey_size(receiver) != AKEYSIZ) {
		return -1;
	}

	// check proof value
	if (proof_verify(pack->body.hash, diff, pack->body.proof) != 0) {
		return -2;
	}

	// decrypt session key
	if (akey_decrypt(receiver, pack->head.sesnk, pack->head.sesnk) != 0) {
		return -3;
	}

	// decrypt public key
	crypto_encrypt(pack->head.sendr+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->head.sendr, IVECSIZ, 
		pack->head.sendr+IVECSIZ, AKEYSIZ*3);

	pubkey = _load_public_key(pack->head.sendr+IVECSIZ, AKEYSIZ);
	if (pubkey == NULL) {
		return -4;
	}

	retcode = 0;

	// decrypt head.title, head.rands
	crypto_encrypt(pack->head.title+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->head.title, IVECSIZ, 
		pack->head.title+IVECSIZ, 32);

	crypto_encrypt(pack->head.rands+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->head.rands, IVECSIZ, 
		pack->head.rands+IVECSIZ, 32);

	// decrypt body.data, body.sign
	crypto_encrypt(pack->body.data+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->body.data,  IVECSIZ, 
		pack->body.data+IVECSIZ, 512);

	crypto_encrypt(pack->body.sign+IVECSIZ, 
		pack->head.sesnk, SKEYSIZ, 
		pack->body.sign,  IVECSIZ, 
		pack->body.sign+IVECSIZ, AKEYSIZ*2);

	_store_public_key(receiver, tmparr, AKEYSIZ);

	// generate hash
	crypto_hash(hashes[0], pack->head.rands+IVECSIZ, 32);
	crypto_hash(hashes[1], pack->head.sendr+IVECSIZ, AKEYSIZ*3);
	crypto_hash(hashes[2], tmparr, AKEYSIZ*3);
	crypto_hash(hashes[3], pack->head.title+IVECSIZ, 32);
	crypto_hash(hashes[4], pack->body.data+IVECSIZ, 512);

	memset(checkhash, 0, HASHSIZ);
	for (int i = 0; i < 5; ++i) {
		memcpy(tmparr, checkhash, HASHSIZ);
		memcpy(tmparr+HASHSIZ, hashes[i], HASHSIZ);
		crypto_hash(checkhash, tmparr, HASHSIZ*2);
	}

	// check hash
	if (memcmp(pack->body.hash, checkhash, HASHSIZ) != 0) {
		akey_free(pubkey);
		return -5;
	}

	// check sign
	if (akey_verify(receiver, pack->body.sign+IVECSIZ, pack->body.hash, HASHSIZ) != 0) {
		akey_free(pubkey);
		return -6;
	}

	pack->temp.sendr = pubkey;

	return retcode;
}

extern akey_t *package_sender(package_t *pack) {
	return pack->temp.sendr;
}

extern unsigned char *package_title(package_t *pack) {
	return pack->head.title+IVECSIZ;
}

extern unsigned char *package_data(package_t *pack) {
	return pack->body.data+IVECSIZ;
}

static void _store_public_key(akey_t *pubkey, uint8_t *output, int ksize) {
	memset(output, 0, ksize*3);
	bigint_save(akey_p(pubkey), output, ksize);
	bigint_save(akey_g(pubkey), output+ksize, ksize);
	bigint_save(akey_y(pubkey), output+(ksize*2), ksize);
}

static akey_t *_load_public_key(uint8_t *input, int ksize) {
	akey_t *pubkey;

	bigint_t *p = bigint_new("0");
	bigint_t *g = bigint_new("0");
	bigint_t *y = bigint_new("0");

	bigint_load(p, input, ksize);
	bigint_load(g, input+ksize, ksize);
	bigint_load(y, input+(ksize*2), ksize);

	pubkey = akey_set(ksize, AKEY_PUBLIC, y, g, p);

	bigint_free(p);
	bigint_free(g);
	bigint_free(y);

	return pubkey;
}
