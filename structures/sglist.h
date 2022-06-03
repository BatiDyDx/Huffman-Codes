#ifndef __SGLIST_H__
#define __SGLIST_H__

#include "../common.h"

// Listas generales ordenadas
typedef struct _SGNode {
    void* data;
    struct _SGNode* next;
} *SGList;

/*
* Retorna una lista vacia
*/
SGList sglist_init();

/*
* Destruye la lista, liberando memoria
*/
void sglist_free(SGList list, DestroyFunction destroy);

/*
* Retorna si la lista es vacia
*/
int sglist_empty(SGList list);

/*
* Inserta el una copia del dato dado en la lista en la posicion indicada
* segun la funcion de comparación
*/
SGList sglist_insert(SGList list, void *data, CopyFunction copy,
                        CompareFunction cmp);

#endif // __SGLIST_H__