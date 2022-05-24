#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/*
Function that returns a pointer to a copy of data
*/
typedef void* (*CopyFunction)(void* data);

/*
* Returns 0 if both arguments are equal, a negative integer if
* first arguements is previous to the second, and a positive
* integer if second arguments is previous to the first one
*/
typedef int (*CompareFunction)(void*, void*);

/*
* Function to destroy data associated to a data structure
*/
typedef void (*DestroyFunction)(void* data);


#endif /* __COMMON_H__ */