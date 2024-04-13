#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "container/map.h"
#include "lib/unitest.h"

#define NUM_THREADS 4

void *map_thread_func(void *arg) {
    HashTable *table = (HashTable *)arg;
    for (int i = 0; i < 10; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        int *value = malloc(sizeof(int));
        *value = i;
        if (hash_table_insert(table, key, value) != 0) {
            fprintf(stderr, "Failed to insert key-value pair\n");
            free(value);
            continue;
        }
        printf("Inserted %s: %d\n", key, *value);
        sleep(1);
        void *result = hash_table_get(table, key);
        if (result == NULL) {
            fprintf(stderr, "Failed to get value for key %s\n", key);
        } else {
            printf("Got %s: %d\n", key, *(int *)result);
        }
        if (hash_table_remove(table, key) != 0) {
            fprintf(stderr, "Failed to remove key %s\n", key);
        } else {
            printf("Removed %s\n", key);
        }
        free(value);
    }
    return NULL;
}

UNITEST(container_map_test) {
    HashTable *table = hash_table_create(8);
    unitest_assert(table != NULL, "failed to create hash table");
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, map_thread_func, table);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    hash_table_destroy(table);
}
