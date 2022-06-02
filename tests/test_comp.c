#include "test_comp.h"
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

    CharFreq cpy_frequencies[len];
    create_frequencies(cpy_frequencies, NCHARS);
    calculate_freq(str, len, cpy_frequencies);
    
    // Los caracteres que tengan mayor frecuencia quedaran al final de 
    // nuestro array de frecuencias ordenado

    //En frequencies[255] tenemos (" ", 7) y en cpy_frequencies[255] (ÿ, 0)
    assert(freqs[255]->freq > cpy_frequencies[255]->freq);

    // En frequencies[253] tenemos (a,4) y en cpy_frequencies[253] (ý,0)
    assert(freqs[253]->freq > cpy_frequencies[253]->freq);

    // En frequencies[249] tenemos (u,2) y en cpy_frequencies[253] (ù,0)
    assert(freqs[249]->freq > cpy_frequencies[249]->freq);

    // En frequencies[246] tenemos (m,2) y en cpy_frequencies[246] (ö,0)
    assert(freqs[246]->freq > cpy_frequencies[249]->freq);
}

void test_compare_node_frequencies() {

    CharFreq freq1 = malloc(sizeof(CharFreq));
    CharFreq freq2 = malloc(sizeof(CharFreq));
    freq1->freq = 3;
    freq2->freq = 10;
    
    BTree tree1 = btree_join(freq1, NULL, NULL, (CopyFunction) id);
    BTree tree2 = btree_join(freq2, NULL, NULL, (CopyFunction) id);

    assert(compare_nodes_freq(tree1, tree2) == -7);

    freq1->freq = 10;
    freq2->freq = 4;
    assert(compare_nodes_freq(tree1, tree2) == 6);

    freq2->freq = 10;
    assert(compare_nodes_freq(tree1, tree2) == 0);

    free(tree1);
    free(tree1);
}

SGList create_test_list(CharFreq* frequencies){
    
    SGList test_list = sglist_init();
    BTree tmp;

    tmp = btree_join(frequencies[3], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id, (CompareFunction)compare_nodes_freq);

    tmp = btree_join(frequencies[1], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id, (CompareFunction)compare_nodes_freq);

    tmp = btree_join(frequencies[0], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id, (CompareFunction)compare_nodes_freq);

    tmp = btree_join(frequencies[2], NULL, NULL, (CopyFunction) id);
    test_list = sglist_insert(test_list, tmp, id, (CompareFunction)compare_nodes_freq);

    return test_list;
}

// void test_create_nodes_from_array() {

//     // Nuestra cadena de prueba será "aabbbcdddd"

//     CharFreq frequencies[4];
//     CharFreq frq1 = malloc(sizeof(CharFreq));
//     CharFreq frq2 = malloc(sizeof(CharFreq));
//     CharFreq frq3 = malloc(sizeof(CharFreq));
//     CharFreq frq4 = malloc(sizeof(CharFreq));
//     frq1->c = 'a';
//     frq1->freq = 2;
//     frq2->c = 'b';
//     frq2->freq = 3;
//     frq3->c = 'c';
//     frq3->freq = 1;
//     frq4->c = 'd';
//     frq4->freq = 4;
//     frequencies[0] = frq1;
//     frequencies[1] = frq2;
//     frequencies[2] = frq3;
//     frequencies[3] = frq4;

//     SGList test_list1 = create_test_list(frequencies);
//     SGList test_list2 = create_nodes_from_array(frequencies, 4);

//     assert(((CharFreq)(test_list1->data))->freq == ((CharFreq)(test_list2->data))->freq);
    
//     assert(((CharFreq)(test_list1->next->data))->freq == 
//     ((CharFreq)(test_list2->next->data))->freq);
    
//     assert(((CharFreq)(test_list1->next->next->data))->freq == 
//     ((CharFreq)(test_list2->next->next->data))->freq);

//     sglist_free(test_list1, free);
//     sglist_free(test_list2, free);
// }

// void test_serialize_tree_and_nodes() {

// }

// void test_create_huff_tree() {
//     // Cadena de prueba: "Ciencias de la computacion en la FCEIA"
//     char chars_str[20] = {
//                         'C', 'i', 'e', 'n', 'c', 'a', 's', 
//                         ' ', 'd', 'l', 'o', 'm', 'p', 'u',
//                         't', 'F', 'E', 'I', 'A', '\0'};
//     size_t frequencies_str[20] = {
//                         2, 2, 2, 3, 2, 4, 1,
//                         6, 1, 2, 2, 1, 1, 1,
//                         1, 1, 1, 1, 1, 1};
//     CharFreq* char_freq_str = malloc(sizeof(CharFreq) * 20);
//     assert(char_freq_str != NULL);
//     for (int i = 0; i < 20; i++) {
//         char_freq_str[i] = malloc(sizeof(struct _CharFreq));
//         assert(char_freq_str[i] != NULL);
//         char_freq_str[i]->c = (UChar) chars_str[i];
//         char_freq_str[i]->freq = frequencies_str[i];
//     }
//     BTree root = create_huff_tree(char_freq_str, 20);

//     btree_destroy(root, free);
//     free(chars_str);
// }

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

    assert(memcmp(tree_encoding, "010100111 !4c1", 16) == 0);
    
    btree_destroy(huff_tree, free);
    for (int i = 0; i < 5; i++)
        free(frequencies[i]);
    free(tree_encoding);
    btree_destroy(huff_tree, free);
}

void run_comp_tests() {
    test_calculate_freq();
    test_sort_freq();
    //test_create_nodes_from_array();
    //test_compare_node_frequencies();
    // test_encode_tree();
    //test_create_huff_tree();
}