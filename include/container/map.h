#ifndef CONTAINER_MAP_H
#define CONTAINER_MAP_H

#include <stddef.h>
#include <pthread.h>

typedef struct HashNode {
    char *key;
    void *value;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    size_t size;
    HashNode **buckets;
    pthread_mutex_t *locks;
} HashTable;

HashTable *hash_table_create(size_t size);
void hash_table_destroy(HashTable *table);
int hash_table_insert(HashTable *table, char *key, void *value);
void *hash_table_get(HashTable *table, char *key);
int hash_table_remove(HashTable *table, char *key);

#endif
