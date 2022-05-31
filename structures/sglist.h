#ifndef __SGLIST_H__
#define __SGLIST_H__

#include "common.h"

// Sorted General List
typedef struct _SGNode {
    void* data;
    struct _SGNode* next;
} *SGList;


/*
* Returns an empty list
*/
SGList sglist_init();

/*
* Destroys the list
*/
void sglist_free(SGList list, DestroyFunction destroy);

/*
* Returns whether the list is empty
*/
int sglist_empty(SGList list);

/*
* Inserts the given data in the list in sorted list, using the
* criterion stablished by the cmp function.
*/
SGList sglist_insert(SGList list, void *data, CopyFunction copy, CompareFunction cmp);

#endif // __SGLIST_H__