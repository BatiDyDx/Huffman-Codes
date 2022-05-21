#ifndef __COMP_H__
#define __COMP_H__

#include <stdio.h>
#include "structures/btree.h"

#define CHARS 256

typedef struct {
    unsigned char c;
    size_t freq;
} *CharFreq;


/*
 ** 
*/
CharFreq* calculate_freq(FILE* fp);


/*
Returns a negative integer if the frequency from the first structure
is less than the frequency in the second one, 0 if ther are equal, or
a positive integer otherwise
*/
int compare_freq(CharFreq ch1, CharFreq ch2);




/*
 ** 
*/
void compress(const char* filename);


#endif /* __COMP_H__ */