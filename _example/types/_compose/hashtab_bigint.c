#include "extclib/io.h"
#include "extclib/types.h"

int main(void) {
    HashTab *hashtab = new_hashtab(250, STRING_TYPE, BIGINT_TYPE);

    set_hashtab(hashtab, string("varX"), new_bigint("872138712637512787387124821738712648712736128749182"));
    set_hashtab(hashtab, string("varY"), new_bigint("675346126835124712346172467268375128731"));
    set_hashtab(hashtab, string("result"), new_bigint(""));

    // result <- varX * varY
    mul_bigint(
        get_hashtab(hashtab, string("result")).bigint, 
        get_hashtab(hashtab, string("varX")).bigint,
        get_hashtab(hashtab, string("varY")).bigint
    );

    printf_io("(%B * %B = %B)\n", 
        get_hashtab(hashtab, string("varX")).bigint,
        get_hashtab(hashtab, string("varY")).bigint,
        get_hashtab(hashtab, string("result")).bigint
    );
    free_hashtab(hashtab);
    return 0;
}
