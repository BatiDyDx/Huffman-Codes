#ifndef __BTREE_H__
#define __BTREE_H__

#include "common.h"

typedef enum {
	BTREE_INORDER,
	BTREE_PREORDER,
	BTREE_POSTORDER
} BTreeVisitOrder;

struct _BTNode {
	void* data;
	struct _BTNode *left;
	struct _BTNode *right;
};

typedef struct _BTNode *BTree;

/*
** Returns an empty tree
*/
BTree btree_init();

/*
** Destroys the tree
*/
void btree_destroy(BTree tree, DestroyFunction destroy);

/*
** Indicates if the tree is empty
*/
int btree_empty(BTree tree);

/*
** Creates a new tree, with the data given in the root node, and the subtrees
** given on the left and right
*/
BTree btree_join(void* data, BTree left, BTree right);

/*
** Visits all nodes in the tree applying the visit function,
** in the given order (Depth first search algorithm)
*/
void btree_visit(BTree tree, BTreeVisitOrder order,
                void (*visit)(void* data));

/*
** Returns 1 if the node is a leaf, 0 if not or -1 if it is empty
*/
int btree_leaf(BTree tree);

/*
** Returns the number of nodes the tree contains
*/
int btree_nnodes(BTree tree);

/*
** Returns 1 if the number given is in the tree, 0 otherwise
*/
int btree_search(BTree tree, void* data, CompareFunction cmp);

/*
** Returns a copy of the tree
*/
BTree btree_copy(BTree tree);

/*
** Returns the height of the tree
*/
int btree_height(BTree tree);

#endif /* __BTREE_H__ */