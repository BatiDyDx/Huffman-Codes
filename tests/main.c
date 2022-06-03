#include "../src/comp.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

static inline void* id(void* p) { return p; }

void test_calculate_freq() {
    char* str = "Esto es una cadena de prueba para testing";

    int len = strlen(str) + 1;
    CharFreq frequencies[NCHARS];
    create_frequencies(frequencies, NCHARS);
    calculate_freq(str, len, frequencies);

    assert(frequencies[(UChar) '\0']->freq == 1);
    assert(frequencies[(UChar) 'o']->freq == 1);
    assert(frequencies[(UChar) 'E']->freq == 1);
    assert(frequencies[(UChar) 'u']->freq == 2);
    assert(frequencies[(UChar) ' ']->freq == 7);
    assert(frequencies[(UChar) 'a']->freq == 6);
    assert(frequencies[(UChar) 'e']->freq == 5);

    for (int i = 0; i < NCHARS; i++)
        free(frequencies[i]);
}

void test_sort_freq() {

    char* str = "esto es un test para el algoritmo de Huffman";
    /*
    Las frecuencias de este string nos quedan:
    (e,5), (s,3), (t,3), (o,3), (u,2), (n,2), (p,1), (a,4), (l,1), (g,1), (r,2)
    (i,1), (m,2), (d,1), (H,1), (f,2), (n,1), (\0,1), (" ", 7)

    Que ordenadas nos quedan:
    (\0,1), (d,1), (g,1), (H,1), (i,1), (l,1), (n,1), (p,1), (f, 2)
    (m,2), (n,2), (r,2), (u,2), (o,3), (s,3), (t,3), (a,4), (e,5), (" ", 7)
    */
    int len = strlen(str) + 1; // Incluimos '\0' tambien
    CharFreq freqs[NCHARS];
    create_frequencies(freqs, NCHARS);
    calculate_freq(str, len, freqs);
    sort_freq(freqs, NCHARS);

    CharFreq unsorted_freqs[NCHARS];
    create_frequencies(unsorted_freqs, NCHARS);
    calculate_freq(str, len, unsorted_freqs);
    
    // Los caracteres que tengan mayor frecuencia quedaran al final de 
    // nuestro array de frecuencias ordenado

    //En frequencies[255] tenemos (" ", 7) y en unsorted_freqs[255] (ÿ, 0)
    assert(freqs[255]->freq > unsorted_freqs[255]->freq);

    // En frequencies[253] tenemos (a,4) y en unsorted_freqs[253] (ý,0)
    assert(freqs[253]->freq > unsorted_freqs[253]->freq);

    // En frequencies[249] tenemos (u,2) y en unsorted_freqs[253] (ù,0)
    assert(freqs[249]->freq > unsorted_freqs[249]->freq);

    // En frequencies[246] tenemos (m,2) y en unsorted_freqs[246] (ö,0)
    assert(freqs[246]->freq > unsorted_freqs[249]->freq);
    for (int i = 0; i < NCHARS; i++) {
        free(freqs[i]);
        free(unsorted_freqs[i]);
    }
}

SGList aux_create_list(CharFreq frequencies[4]){
    SGList test_list = sglist_init();
    BTree tmp;

    
    
    tmp = btree_join(frequencies[1], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id,
                                (CompareFunction)compare_nodes_freq);

    tmp = btree_join(frequencies[2], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id,
                                (CompareFunction)compare_nodes_freq);

    tmp = btree_join(frequencies[3], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id,
                                (CompareFunction)compare_nodes_freq);

    return test_list;
}

void test_create_nodes() {

    // Nuestra cadena de prueba será "aabbbcdddd"
    CharFreq frequencies[4];
    for (int i = 0; i < 4; i++) {
        frequencies[i] = malloc(sizeof(struct _CharFreq));
        assert(frequencies[i] != NULL);
    }

    frequencies[0]->c = 'a';
    frequencies[0]->freq = 2;
    frequencies[1]->c = 'b';
    frequencies[1]->freq = 3;
    frequencies[2]->c = 'c';
    frequencies[2]->freq = 1;
    frequencies[3]->c = 'd';
    frequencies[3]->freq = 4;

    SGList test_list = create_nodes(frequencies, 4);

    assert(((CharFreq)((BTree)(test_list->data))->data)->freq == 1);
    assert(((CharFreq)((BTree)(test_list->next->data))->data)->freq == 2);
    assert(((CharFreq)((BTree)(test_list->next->next->data))->data)->freq == 3);
    assert(((CharFreq)((BTree)(test_list->next->next->next->data))->data)->freq
            == 4);

    sglist_free(test_list, free);
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

    assert(memcmp(tree_encoding, "001001111!4c1 ", 15) == 0);
    
    free(tree_encoding);
    btree_destroy(huff_tree, free);
}

int main() {
    test_calculate_freq();
    test_sort_freq();
    test_create_nodes();
    test_encode_tree();
    return 0;
}
