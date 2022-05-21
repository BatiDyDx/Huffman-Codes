#ifndef _SGLIST_H_
    #define _SGLIST_H_

    #include "glist.h"
    #include <stdlib.h>
    #include <assert.h>

    // Sorted General List
    typedef GList SGList;
    
    /*
     * Returns 0 if both arguments are equal, a negative integer if
     * first arguements is previous to the second, and a positive
     * integer if second arguments is previous to the first one
    */
    typedef int (*CompareFunction)(void*, void*);

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
     * Visits all the elements in the list, applying the f function
    */
    void sglist_visit(SGList list, VisitFunction f);

    /*
     * Inserts the given data in the list in sorted list, using the
     * criterion stablished by the cmp function
    */
    SGList sglist_insert(SGList list, void *data, CopyFunction copy, CompareFunction cmp);

    /*
     * Searches for an element in the sorted list
    */
    int sglist_search(SGList list, void *data, CompareFunction cmp);

    /*
     * Takes an array and generates a sorted general list
     * from the elements of the array, using the order given
     * by the cmp function.
    */
    SGList sglist_from_array(void **arr, size_t len, CopyFunction copy, CompareFunction cmp);

#endif // _SGLIST_H_