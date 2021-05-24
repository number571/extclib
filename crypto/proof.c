#include "../crypto.h"
#include "../encoding.h"
#include "../type/bigint.h"

#include <stdint.h>
#include <string.h>

extern unsigned long long proof_of_work (
	const unsigned char hash[32], 
	unsigned char diff
) {
	uint8_t  thash[40];
	uint64_t proof;
	bigint_t *hashnum;
	bigint_t *diffnum;

	hashnum = bigint_new("0");
	diffnum = bigint_new("1");

	bigint_shl(diffnum, diffnum, 256-diff);

	for (proof = 0; proof < UINT64_MAX; ++proof) {
		memcpy(thash, hash, 32);
		split_64bits_to_8bits(proof, thash+32);

		crypto_hash(thash, thash, 40);
		bigint_load(hashnum, thash, 32);

		if (bigint_cmp(hashnum, diffnum) < 0) {
			break;
		}
	}

	bigint_free(hashnum);
	bigint_free(diffnum);

	return proof;
}

extern int proof_verify (
	const unsigned char hash[32],
	unsigned char diff,
	unsigned long long proof
) {
	int retcod;
	uint8_t  thash[40];
	bigint_t *hashnum;
	bigint_t *diffnum;

	hashnum = bigint_new("0");
	diffnum = bigint_new("1");

	bigint_shl(diffnum, diffnum, 256-diff);
	memcpy(thash, hash, 32);
	split_64bits_to_8bits(proof, thash+32);

	crypto_hash(thash, thash, 40);
	bigint_load(hashnum, thash, 32);

	if (bigint_cmp(hashnum, diffnum) < 0) {
		retcod = 0;
	} else {
		retcod = 1;
	}

	bigint_free(hashnum);
	bigint_free(diffnum);

	return retcod;
}
