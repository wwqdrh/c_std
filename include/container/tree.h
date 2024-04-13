#ifndef CONTAINER_TREE_H
#define CONTAINER_TREE_H

#include <stdatomic.h>
#include <stdbool.h>

typedef struct bst_node {
	int val;
	struct bst_node* left;
	struct bst_node* right;
	_Atomic unsigned long lock;
} bst_node_t;

typedef struct {
	bst_node_t* root;
} bst_t;

bst_t* bst_create();
bool bst_insert(bst_t* tree, int val);
bool bst_search(bst_t* tree, int val);
void bst_destroy(bst_t* tree);
void bst_inorder_traversal(bst_t* tree, void (*callback)(int));

#endif
