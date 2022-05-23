#ifndef __BTREE_H__
#define __BTREE_H__

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
 ** btree_visit_bfs_recursive:
 ** Recursively visits all nodes in the tree applying the visit function,
 ** using the breath first search algorithm
 ** btree_visit_n_depth:
 ** Applies the visit function only to nodes with the depth given
*/
void btree_visit_bfs_recursive(BTree tree, void (*visit)(void* data));

void btree_visit_n_depth(BTree tree, int depth, void (*visit)(void* data));

/*
 ** Iteratively visits all nodes in the tree applying the visit function,
 ** using the breath first search algorithm
*/
void btree_visit_bfs_iter(BTree tree, void (*visit)(int data));

/*
 ** Visits all nodes in the tree applying the visit function
 ** with an iterative method in pre-order
*/
void btree_preorder_iter(BTree tree, void (*visit)(int data));

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
int btree_search(BTree tree, int data);

/*
 ** Returns a copy of the tree
*/
BTree btree_copy(BTree tree);

/*
 ** Returns the height of the tree
*/
int btree_height(BTree tree);

/*
 ** Returns the number of nodes in the tree with the depth given
*/
int btree_nnodes_depth(BTree tree, int depth);

/*
 ** Returns the depth of the node that contains the number passed,
 ** or -1 if it is not in the tree
*/
int btree_depth(BTree tree, int data);

/*
 ** Returns the sum of the nodes in the tree
*/
int btree_sum(BTree tree);

#endif /* __BTREE_H__ */
