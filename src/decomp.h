#ifndef __DECOMP_H__
#define __DECOMP_H__

#include "../structures/btree.h"

BTree deserialize_tree(char** encoded_tree, char** encoded_values);

BTree tree_from_encoding(char* encoded_tree);

char* decode_text(BTree root, char* encoded_text, const int encoded_len, int* decode_len);

void decompress(const char* path);

/*
Given a .hf file of a compressed text,
decompress it using the huffman algorithm.
Depends on the existence of a .tree file that
indicates the tree used to encode the text
*/
// void decompress(const char* filename);

#endif /* __DECOMP_H__ */