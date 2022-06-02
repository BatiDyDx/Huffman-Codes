#ifndef __BTREE_H__
#define __BTREE_H__

#include "../common.h"

struct _BTNode {
	void* data;
	struct _BTNode *left;
	struct _BTNode *right;
};

typedef struct _BTNode *BTree;

/*
** Retorna un arbol vacio
*/
BTree btree_init();

/*
** Destruye el arbol, liberando memoria
*/
void btree_destroy(BTree tree, DestroyFunction destroy);

/*
** Retorna si el arbol es vacio
*/
int btree_empty(BTree tree);

/*
** Crea un nuevo arbol, con el dato dado en la raiz, y los arboles
** dados como argumentos como sus hijos
*/
BTree btree_join(void* data, BTree left, BTree right);

/*
 * Retorna la altura del arbol
*/
int btree_height(BTree tree);

#endif /* __BTREE_H__ */