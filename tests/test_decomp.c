#include "test_decomp.h"
#include <assert.h>

void f(void* c) {
    if (c)
        putchar(*((int*) c));
}

void test_tree_from_encoding() {

    BTree tree = tree_from_encoding("01011\0abc");
    btree_visit(tree, BTREE_PREORDER, f);

    btree_destroy(tree, f);
}


void run_decomp_tests() {
    test_tree_from_encoding();
}