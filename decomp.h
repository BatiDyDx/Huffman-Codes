#ifndef __DECOMP_H__
#define __DECOMP_H__

/*
Given a .hf file of a compressed text,
decompress it using the huffman algorithm.
Depends on the existence of a .tree file that
indicates the tree used to encode the text
*/
void decompress(const char* filename);

#endif /* __DECOMP_H__ */