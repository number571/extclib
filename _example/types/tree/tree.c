#include <stdio.h>

#include "extclib/types/tree.h"

int main(void) {
    Tree *tree = new_tree(CHARS_TYPE, DECIMAL_TYPE);

    set_tree(tree, chars("B"), decimal(10));
    set_tree(tree, chars("A"), decimal(20));
    set_tree(tree, chars("C"), decimal(30));

    del_tree(tree, chars("B"));

    char *elem = "A";
    if (in_tree(tree, chars(elem))) {
        printf("%d\n", get_tree(tree, chars(elem)).decimal);
    }

    println_tree(tree);
    println_tree_branches(tree);
    
    free_tree(tree);
    return 0;
}
