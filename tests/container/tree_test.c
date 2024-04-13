#include <stdio.h>
#include <pthread.h>

#include "container/tree.h"
#include "lib/unitest.h"

#define NUM_THREADS 4

bst_t* tree;

void* insert_task(void* arg) {
    int val = *(int*)arg;
    printf("Inserting value %d\n", val);
    if (!bst_insert(tree, val)) {
        printf("Failed to insert value %d\n", val);
    }
    return NULL;
}

void* search_task(void* arg) {
    int val = *(int*)arg;
    printf("Searching for value %d\n", val);
    if (bst_search(tree, val)) {
        printf("Value %d found\n", val);
    } else {
        printf("Value %d not found\n", val);
    }
    return NULL;
}

static inline void print_value(int val) {
    printf("%d ", val);
}

UNITEST(tree_thread_test) {
    tree = bst_create();
    unitest_assert(tree != NULL, "failed create BST");
    
    int values[] = {5, 3, 7, 1, 4, 6, 8, 2};
    int num_values = sizeof(values) / sizeof(int);

    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    // 插入值
    for (int i = 0; i < num_values; i++) {
        thread_args[i % NUM_THREADS] = values[i];
        pthread_create(&threads[i % NUM_THREADS], NULL, insert_task, &thread_args[i % NUM_THREADS]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 搜索值
    thread_args[0] = 5;
    thread_args[1] = 3;
    thread_args[2] = 9;
    thread_args[3] = 6;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, search_task, &thread_args[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Inorder traversal: ");
    bst_inorder_traversal(tree, print_value);
    printf("\n");

    bst_destroy(tree);

}
