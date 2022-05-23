#ifndef __COMP_H__
#define __COMP_H__

#include <stdio.h>
#include "../structures/btree.h"
#include "../structures/sglist.h"

#define CHARS 256

struct _CharFreq
{
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

CharFreq* create_frequencies();

void free_frequencies(CharFreq* frequencies);

CharFreq* calculate_freq(char* file_content, int len);

SGList create_nodes_from_array(CharFreq* frequencies);

BTree create_huff_tree(CharFreq* frequencies);

void compress(const char *filename);

#endif /* __COMP_H__ */