#ifndef __GLIST_H__
#define __GLIST_H__

#include "common.h"

typedef struct _GNode {
  void* data;
  struct _GNode* next;
} GNode;

typedef GNode* GList;

/*
** Returns an empty list
*/
GList glist_init();

/*
** Destroys the list
*/
void glist_free(GList list, DestroyFunction destroy);

/*
** Returns 1 if the list is empty, 0 otherwise
*/
int glist_empty(GList list);

/*
** Adds an element to the start of the list
*/
GList glist_append_start(GList list, void* data, CopyFunction copy);

/*
** Removes the first element of the list if its not empty, and returns the
** new first node
*/
GList glist_remove_start(GList list, DestroyFunction destroy);


#endif /* __GLIST_H__ */
