#include "btree.h"
#include "./gqueue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(X, Y) (X > Y ? X : Y)

struct _BTNode {
	int data;
	struct _BTNode *left;
	struct _BTNode *right;
};


BTree btree_init() { return NULL; }

void btree_destroy(BTree tree) {
	if (tree != NULL) {
		btree_destroy(tree->left);
		btree_destroy(tree->right);
		free(tree);
	}
}

int btree_empty(BTree tree) { return tree == NULL; }

BTree btree_join(int data, BTree left, BTree right) {
	BTree new_node = malloc(sizeof(struct _BTNode));
	assert(new_node != NULL);
	new_node->data = data;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

void btree_visit(BTree tree, BTreeVisitOrder order,
					void (*visit)(int data)) {
	if (tree == NULL)
		return;
	if (order == BTREE_INORDER) {
		btree_visit(tree->left, order, visit);
		visit(tree->data);
		btree_visit(tree->right, order, visit);
	} else if (order == BTREE_PREORDER) {
		visit(tree->data);
		btree_visit(tree->left, order, visit);
		btree_visit(tree->right, order, visit);
	} else if (order == BTREE_POSTORDER) {
		btree_visit(tree->left, order, visit);
		btree_visit(tree->right, order, visit);
		visit(tree->data);
	} else {
		fprintf(stderr, "Order not valid for visiting a tree\n");
		exit(1);
	}
	return;
}

/*
 ** Function to "copy" a BTree, since a copy function
 ** for pushing to a general stack is required
*/
void* id(void* x) {
	return x;
}

/*
 ** Function to "destroy" a BTree, since a destroy function
 ** for poping out of a general stack is required.
*/
void null(__attribute__((unused)) void* x) { return; }

void btree_preorder_iter(BTree tree, void (*visit)(int data)) {

	GStack stack = gstack_init();

	if (btree_empty(tree))
		return;

	while (1) {
		visit(tree->data);
		
		if (tree->right != NULL)
			stack = gstack_push(stack, tree->right, (CopyFunction)id);
		if (tree->left != NULL)
			stack = gstack_push(stack, tree->left, (CopyFunction)id);

		if (gstack_empty(stack))
			break;

		tree = (BTree)gstack_top(stack, (CopyFunction)id);
		stack = gstack_pop(stack, (DestroyFunction)null);
	}

	// Although it is supposed to be empty,
	// it is a good practica to free the stack anyways
	gstack_free(stack, (DestroyFunction)null);
}

void btree_visit_bfs_iter(BTree tree, void (*visit)(int data)) {
	GQueue queue = gqueue_init();

	if (btree_empty(tree))
		return;

	do {
		visit(tree->data);
		if (!btree_empty(tree->left))
			gqueue_push(queue, tree->left, (CopyFunction)id);
		if (!btree_empty(tree->right))
			gqueue_push(queue, tree->right, (CopyFunction)id);
		if (gqueue_empty(queue))
			break;
		tree = (BTree) gqueue_start(queue, (CopyFunction)id);
		gqueue_pop(queue, (DestroyFunction)null);
	} while (1);

	// Although it is supposed to be empty,
	// it is a good practica to free the queue anyways
	gqueue_free(queue, (DestroyFunction)null);
}

int btree_leaf(BTree tree) {
	if (btree_empty(tree))
		return -1;
	return btree_empty(tree->left) && btree_empty(tree->right);
}

int btree_nnodes(BTree tree) {
	if (btree_empty(tree))
		return 0;
	return btree_nnodes(tree->left) + btree_nnodes(tree->right) + 1;
}

int btree_search(BTree tree, int data) {
	if (btree_empty(tree))
		return 0;
	else if (tree->data == data)
		return 1;
	else
		return (btree_search(tree->left, data) || \
				btree_search(tree->right, data) );
}

BTree btree_copy(BTree tree) {
	if (btree_empty(tree))
		return NULL;
	BTree node_copy = malloc(sizeof(struct _BTNode));
	assert(node_copy != NULL);

	node_copy->data = tree->data;
	node_copy->left = btree_copy(tree->left);
	node_copy->right = btree_copy(tree->right);

	return node_copy;
}

int btree_height(BTree tree) {
	if (btree_empty(tree))
		return -1;
	if (btree_leaf(tree))
		return 0;
	return MAX(btree_height(tree->left), btree_height(tree->right)) + 1;
}

int btree_nnodes_depth(BTree tree, int depth) {
	if (btree_empty(tree))
		return 0;
	if (depth == 0)
		return 1;
	else
		return (btree_nnodes_depth(tree->left, depth - 1) + \
				btree_nnodes_depth(tree->right, depth - 1) );
}

int btree_depth(BTree tree, int data) {
	int depth, tmp;
	if (btree_empty(tree))
		depth = -1;
	else if (tree->data == data)
		depth = 0;
	else if (	(tmp = btree_depth(tree->left, data)) != -1 || \
				(tmp = btree_depth(tree->right, data))!= -1)
		depth = 1 + tmp;
	else
		depth = -1;
	return depth;
}

int btree_sum(BTree tree) {
	if (btree_empty(tree))
		return 0;
	return (tree->data 			  + \
			btree_sum(tree->left) + \
			btree_sum(tree->right) );
}

