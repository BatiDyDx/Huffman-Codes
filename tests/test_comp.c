#include "test_comp.h"
#include <string.h>
#include <assert.h>

void test_create_frequencies() {
    CharFreq* array = create_frequencies();
    for (int i = 0; i < CHARS; i++) {
        assert(array[i]->freq == 0);
        assert(array[i]->c == (UChar) i);
    }
    free_frequencies(array);
}

void test_calculate_freq() {
    char* str = "Esto es una cadena de prueba para testing";
    CharFreq* frequencies = calculate_freq(str, strlen(str) + 1);

    assert(frequencies[(UChar) '\0']->freq == 1);
    assert(frequencies[(UChar) 'o']->freq == 1);
    assert(frequencies[(UChar) 'E']->freq == 1);
    assert(frequencies[(UChar) 'u']->freq == 2);
    assert(frequencies[(UChar) ' ']->freq == 7);
    assert(frequencies[(UChar) 'a']->freq == 6);
    assert(frequencies[(UChar) 'e']->freq == 5);
    assert(frequencies[(UChar) EOF]->freq == 0);

    free_frequencies(frequencies);
}

void test_sort_freq() {
    char* str = "Ciencias de la computacion en la FCEIA";
    size_t len = strlen(str) + 1; // Incluimos '\0' tambien
    CharFreq* frequencies = calculate_freq(str, len);
    sort_freq(frequencies, len);

    /* TODO */

    free_frequencies(frequencies);
}

void test_encode_tree() {
    char test_chars[5] = {'c', '1', '4', ' ', '!'};
    size_t test_freqs[5] = {4, 7, 1, 23, 11};
    CharFreq frequencies[5];
    for (int i = 0; i < 5; i++) {
        frequencies[i] = malloc(sizeof(struct _CharFreq));
        assert(frequencies[i] != NULL);
        frequencies[i]->c = (UChar) test_chars[i];
        frequencies[i]->freq = test_freqs[i];
    }

    BTree huff_tree = create_huff_tree(frequencies, 5);
    char* tree_encoding = encode_tree(huff_tree, 5);

    assert(memcmp(tree_encoding, "010100111\0 !4c1", 16) == 0);
    
    btree_destroy(huff_tree, free);
    for (int i = 0; i < 5; i++)
        free(frequencies[i]);
    free(tree_encoding);
}

void run_comp_tests() {
    test_create_frequencies();
    test_calculate_freq();
    test_encode_tree();
}