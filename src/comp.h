#ifndef __COMP_H__
#define __COMP_H__

#include "../structures/btree.h"
#include "../structures/sglist.h"

#define NCHARS 256

typedef unsigned char UChar;

struct _CharFreq {
    UChar c;
    size_t freq;
};

typedef struct _CharFreq *CharFreq;

/*
TODO: remover
*/
SGList create_nodes(CharFreq* frequencies, size_t len);

void create_frequencies(CharFreq buf[NCHARS]);

/*
Llena el buffer de frequencias con las frequencias de cada
caracter en str. El buffer ya debe estar inicializado.
*/
void calculate_freq(char* str, int len, CharFreq frequencies[NCHARS]);

/*
Dado un arreglo de frecuencias ya computadas en un texto, crea el arbol
de Huffman para la codificacion de dichos caracteres.
*/
BTree create_huff_tree(CharFreq* frequencies, int nchars);

void encode_chars(BTree huff_tree, char* chars_encoding[NCHARS],
                    int max_char_len);

char* encode_text(char* text, char** chars_encoding, size_t text_len,
                    int max_char_len, int* len);

/**/
char* encode_tree(BTree huffman_tree, size_t nchars, int* encode_tree);

/*
Takes a path to a file to be compressed using Huffman
algorithm, and writes the result to the file path.hf, and the
corresponding tree used to encode it to the file path.hf.tree
*/
void compress(const char *path, char *hf_path, char *tree_path);

#endif /* __COMP_H__ */