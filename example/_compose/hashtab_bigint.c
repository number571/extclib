#include "extclib/io.h"
#include "extclib/hashtab.h"
#include "extclib/bigint.h"

int main(void) {
    HashTab *hashtab = new_hashtab(250, CHARS_TYPE, BIGINT_TYPE);

    set_hashtab(hashtab, chars("varX"), bigint(new_bigint("872138712637512787387124821738712648712736128749182")));
    set_hashtab(hashtab, chars("varY"), bigint(new_bigint("675346126835124712346172467268375128731")));
    set_hashtab(hashtab, chars("result"), bigint(new_bigint("")));

    // result <- varX * varY
    mul_bigint(
        get_hashtab(hashtab, chars("result")).bigint, 
        get_hashtab(hashtab, chars("varX")).bigint,
        get_hashtab(hashtab, chars("varY")).bigint
    );

    printf_io("(%B * %B = %B)\n", 
        get_hashtab(hashtab, chars("varX")).bigint,
        get_hashtab(hashtab, chars("varY")).bigint,
        get_hashtab(hashtab, chars("result")).bigint
    );
    free_hashtab(hashtab);
    return 0;
}
