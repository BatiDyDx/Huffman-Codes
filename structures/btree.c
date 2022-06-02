#include "btree.h"

BTree btree_init() { return NULL; }

void btree_destroy(BTree tree, DestroyFunction destroy) {
	if (tree != NULL) {
		btree_destroy(tree->left, destroy);
		btree_destroy(tree->right, destroy);
		destroy(tree->data);
		free(tree);
	}
}

int btree_empty(BTree tree) { return tree == NULL; }

BTree btree_join(void* data, BTree left, BTree right) {
	BTree new_node = malloc(sizeof(struct _BTNode));
	assert(new_node != NULL);
	new_node->data = data;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

int btree_height(BTree tree) {
	if (btree_empty(tree))
		return -1;

	if (tree->left == NULL && tree->right == NULL)
		return 0;
	
	int height_left = btree_height(tree->left);
	int height_right = btree_height(tree->right);
	return max(height_left, height_right) + 1;
}
