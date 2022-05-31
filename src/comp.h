#ifndef __COMP_H__
#define __COMP_H__

#include "../structures/btree.h"
#include "../structures/sglist.h"

#define CHARS 256

typedef unsigned char UChar;

struct _CharFreq {
    UChar c;
    size_t freq;
};

typedef struct _CharFreq *CharFreq;

/*
Returns an array of CharFreq where every element
corresponds to a structure containing a character
and the number of times it appears in string passed
*/
void calculate_freq(char* str, int len, CharFreq frequencies[CHARS]);

/*
Given an array of CharFreq, it creates the huffman
tree corresponding to the frequency of the characters in the
file to be compressed.
*/
BTree create_huff_tree(CharFreq* frequencies, size_t nchars);

void encode_chars(BTree huff_tree, char* chars_encoding[CHARS],
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
void compress(const char *path);

#endif /* __COMP_H__ */