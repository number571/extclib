#include <stdio.h>

#include "extclib/crypto/package.h"

int main(void) {
	unsigned char title[32] = {'A', 'B', 'C'};
	unsigned char data[512] = "hello, world!";

	int err;
	akey_t *key;
	package_t *pack;

	key = akey_new(PACKAGE_AKEYSIZ);
	pack = package_new(title, data);

	printf("%s\n", package_body_data(pack));

	err = package_encrypt(pack, key, key, 15);
	if (err != 0) {
		return 1;
	}

	err = package_decrypt(pack, key, 15);
	if (err != 0) {
		return 2;
	}
	printf("%s\n", package_body_data(pack));

	akey_free(key);

	package_free(pack);

    return 0;
}
