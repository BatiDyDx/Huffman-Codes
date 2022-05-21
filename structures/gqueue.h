#ifndef _GQUEUE_H_

    #define _GQUEUE_H_
    #include "./glist.h"

    typedef struct _GQueue{
        GNode *first, *last;
    } *GQueue;

    /*
     ** Returns an empty queue
    */
    GQueue gqueue_init();

    /*
     ** Destroys a queue
    */
    void gqueue_free(GQueue q, DestroyFunction destroy);

    /*
     ** Returns if the queue is empty or not
    */
    int gqueue_empty(GQueue q);

    /*
     ** Returns a copy of the data on the start of the queue
    */
    void* gqueue_start(GQueue q, CopyFunction copy);
    
    /*
     ** Pushes data to the end of the queue
    */
    void gqueue_push(GQueue q, void *data, CopyFunction copy);
    
    /*
     ** Removes the start of the queue
    */
    void gqueue_pop(GQueue q, DestroyFunction destroy);
    
    /*
     ** Prints the queue from the start to the end
    */
    void gqueue_print(GQueue q, VisitFunction print);

#endif // _GQUEUE_H_