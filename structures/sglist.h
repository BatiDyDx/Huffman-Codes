#ifndef __SGLIST_H__
#define __SGLIST_H__

#include "common.h"
#include "glist.h"

// Sorted General List
typedef GList SGList;

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
* criterion stablished by the cmp function
*/
SGList sglist_insert(SGList list, void *data, CopyFunction copy, CompareFunction cmp);

/*
* Searches for an element in the sorted list
*/
int sglist_search(SGList list, void *data, CompareFunction cmp);

#endif // __SGLIST_H__