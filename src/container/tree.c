#include <stdlib.h>

#include "container/tree.h"

static bst_node_t* bst_node_create(int val) {
    bst_node_t* node = malloc(sizeof(bst_node_t));
    if (node == NULL) {
        return NULL;
    }
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    atomic_init(&node->lock, 0);
    return node;
}

static void bst_node_destroy(bst_node_t* node) {
    if (node == NULL) {
        return;
    }
    bst_node_destroy(node->left);
    bst_node_destroy(node->right);
    free(node);
}

bst_t* bst_create() {
    bst_t* tree = malloc(sizeof(bst_t));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    return tree;
}


static bool bst_node_insert(bst_node_t** node, int val) {
    if (*node == NULL) {
        *node = bst_node_create(val);
        return (*node != NULL);
    }

    while (true) {
        atomic_fetch_or_explicit(&(*node)->lock, 1, memory_order_acquire);
        if (val < (*node)->val) {
            if ((*node)->left == NULL) {
                bst_node_t* new_node = bst_node_create(val);
                if (new_node == NULL) {
                    atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
                    return false;
                }
                (*node)->left = new_node;
                atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
                return true;
            }
            atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
            if (!bst_node_insert(&(*node)->left, val)) {
                return false;
            }
        } else if (val > (*node)->val) {
            if ((*node)->right == NULL) {
                bst_node_t* new_node = bst_node_create(val);
                if (new_node == NULL) {
                    atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
                    return false;
                }
                (*node)->right = new_node;
                atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
                return true;
            }
            atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
            if (!bst_node_insert(&(*node)->right, val)) {
                return false;
            }
        } else {
            atomic_fetch_and_explicit(&(*node)->lock, 0, memory_order_release);
            return false;
        }
    }
}

bool bst_insert(bst_t* tree, int val) {
    if (tree == NULL) {
        return false;
    }
    return bst_node_insert(&tree->root, val);
}

static bool bst_node_search(bst_node_t* node, int val) {
    if (node == NULL) {
        return false;
    }

    while (true) {
        atomic_fetch_or_explicit(&node->lock, 1, memory_order_acquire);
        if (val < node->val) {
            atomic_fetch_and_explicit(&node->lock, 0, memory_order_release);
            return bst_node_search(node->left, val);
        } else if (val > node->val) {
            atomic_fetch_and_explicit(&node->lock, 0, memory_order_release);
            return bst_node_search(node->right, val);
        } else {
            atomic_fetch_and_explicit(&node->lock, 0, memory_order_release);
            return true;
        }
    }
}

bool bst_search(bst_t* tree, int val) {
    if (tree == NULL) {
        return false;
    }
    return bst_node_search(tree->root, val);
}

void bst_destroy(bst_t* tree) {
    if (tree == NULL) {
        return;
    }
    bst_node_destroy(tree->root);
    free(tree);
}

static void bst_node_inorder_traversal(bst_node_t* node, void (*callback)(int)) {
    if (node == NULL) {
        return;
    }

    while (true) {
        atomic_fetch_or_explicit(&node->lock, 1, memory_order_acquire);
        bst_node_inorder_traversal(node->left, callback);
        callback(node->val);
        atomic_fetch_and_explicit(&node->lock, 0, memory_order_release);
        bst_node_inorder_traversal(node->right, callback);
        return;
    }
}

void bst_inorder_traversal(bst_t* tree, void (*callback)(int)) {
    if (tree == NULL || callback == NULL) {
        return;
    }
    bst_node_inorder_traversal(tree->root, callback);
}

