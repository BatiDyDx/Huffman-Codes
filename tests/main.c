// #include "test_comp.h"
// #include "test_decomp.h"
#include "../src/comp.h"
#include "../src/decomp.h"
#include "../structures/btree.h"
#include "../structures/sglist.h"
#include <assert.h>

void put(BTree root) {
    printf("%c: %lu\n", ((CharFreq)root->data)->c, ((CharFreq)root->data)->freq);
}

int main() {
    //run_comp_tests();
    //run_decomp_tests();

    CharFreq buf[256];
    create_frequencies(buf);
    buf[(UChar) 'A']->freq = 10;
    buf[(UChar) 'E']->freq = 15;
    buf[(UChar) 'I']->freq = 12;
    buf[(UChar) 'S']->freq = 3;
    buf[(UChar) 'T']->freq = 4;
    buf[(UChar) 'P']->freq = 13;
    buf[(UChar) '\n']->freq = 1;

    // SGList list = create_nodes(buf, NCHARS);
    // sglist_iter(list, (void (*)(void*))put);
    
    BTree root = create_huff_tree(buf, NCHARS);
    assert(((CharFreq) root->data)->freq == 58);
    assert(((CharFreq) root->left->left->data)->c == 'I');
    assert(((CharFreq) root->left->right->data)->c == 'P');
    assert(((CharFreq) root->right->data)->freq == 33);
    assert(((CharFreq) root->right->left->data)->c == 'E');
    assert(((CharFreq) root->right->right->left->data)->c == 'A');
    assert(((CharFreq) root->right->right->right->left->data)->c == 'T');
    assert(((CharFreq) root->right->right->right->right->left->data)->c == '\n');
    assert(((CharFreq) root->right->right->right->right->right->data)->c == 'S');
    
    return 0;
}