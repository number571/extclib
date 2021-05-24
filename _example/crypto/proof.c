#include <stdio.h>
#include <string.h>

#include "extclib/crypto.h"
#include "extclib/crypto/proof.h"

int main(void) {
	unsigned char hash[32];
	char *msg = "hello, world!";
	unsigned long long proof;

	crypto_hash(hash, (unsigned char*)msg, strlen(msg));
	proof = proof_of_work(hash, 20);

	printf("%llu\n", proof);

	if (proof_verify(hash, 20, proof) == 0) {
		printf("success\n");
	} else {
		printf("failed\n");
	}

	return 0;
}
