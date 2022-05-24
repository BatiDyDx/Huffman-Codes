#ifndef __COMP_H__
#define __COMP_H__

#include <stdio.h>
#include "../structures/btree.h"
#include "../structures/sglist.h"

#define CHARS 256

struct _CharFreq {
    unsigned char c;
    size_t freq;
};

typedef struct _CharFreq *CharFreq;

typedef unsigned char UChar;

/*
 **
 */

/*
Returns a negative integer if the frequency from the first structure
is less than the frequency in the second one, 0 if ther are equal, or
a positive integer otherwise
*/
int compare_freq(CharFreq ch1, CharFreq ch2);

/*
Sorts an array of CharFreq structures by its frequency field,
using the quick sort algorithm
*/
void sort_freq(CharFreq *freq_array, int len);

/*
 **
 */
void compress(const char *filename);

/*
* Returns an array of CharFreq, each element
* corresponds to a char with its frequency set to 0
*/
CharFreq* create_frequencies();

/*
Frees the array of CharFreq
*/
void free_frequencies(CharFreq* frequencies);

/*
Returns an array of CharFreq where every element
corresponds to a structure containing a character
and the number of times it appears in string passed
*/
CharFreq* calculate_freq(char* str, int len);

/*
Given a sorted array of CharFreq, it creates the huffman
tree corresponding to the frequency of the characters in the
file to be compressed.
*/
BTree create_huff_tree(CharFreq* frequencies);

/*
Takes a path to a file to be compressed using Huffman
algorithm, and writes the result to the file path.hf, and the
corresponding tree used to encode it to the file path.hf.tree
*/
void compress(const char *path);

#endif /* __COMP_H__ */