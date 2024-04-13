#include <stdlib.h>
#include <string.h>

#include <container/map.h>

#define HASH_FUNC(key, size) (hash_str(key) % size)

unsigned int hash_str(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * 31 + *str++;
    }
    return hash;
}

HashTable *hash_table_create(size_t size) {
    HashTable *table = malloc(sizeof(HashTable));
    if (table == NULL) {
        return NULL;
    }
    table->size = size;
    table->buckets = calloc(size, sizeof(HashNode *));
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }
    table->locks = calloc(size, sizeof(pthread_mutex_t));
    if (table->locks == NULL) {
        free(table->buckets);
        free(table);
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        pthread_mutex_init(&table->locks[i], NULL);
    }
    return table;
}

void hash_table_destroy(HashTable *table) {
    if (table == NULL) {
        return;
    }
    for (size_t i = 0; i < table->size; i++) {
        HashNode *node = table->buckets[i];
        while (node != NULL) {
            HashNode *next = node->next;
            free(node->key);
            free(node);
            node = next;
        }
        pthread_mutex_destroy(&table->locks[i]);
    }
    free(table->buckets);
    free(table->locks);
    free(table);
}

int hash_table_insert(HashTable *table, char *key, void *value) {
    if (table == NULL || key == NULL) {
        return -1;
    }
    size_t index = HASH_FUNC(key, table->size);
    pthread_mutex_lock(&table->locks[index]);
    HashNode *node = table->buckets[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            pthread_mutex_unlock(&table->locks[index]);
            return 0;
        }
        node = node->next;
    }
    HashNode *new_node = malloc(sizeof(HashNode));
    if (new_node == NULL) {
        pthread_mutex_unlock(&table->locks[index]);
        return -1;
    }
    // new_node->key = strdup(key);
    new_node->key = malloc(strlen(key) + 1);
    if (new_node->key == NULL) {
        free(new_node);
        pthread_mutex_unlock(&table->locks[index]);
        return -1;
    }
    strcpy(new_node->key, key);
    new_node->value = value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    pthread_mutex_unlock(&table->locks[index]);
    return 0;
}

void *hash_table_get(HashTable *table, char *key) {
    if (table == NULL || key == NULL) {
        return NULL;
    }
    size_t index = HASH_FUNC(key, table->size);
    pthread_mutex_lock(&table->locks[index]);
    HashNode *node = table->buckets[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            void *value = node->value;
            pthread_mutex_unlock(&table->locks[index]);
            return value;
        }
        node = node->next;
    }
    pthread_mutex_unlock(&table->locks[index]);
    return NULL;
}

int hash_table_remove(HashTable *table, char *key) {
    if (table == NULL || key == NULL) {
        return -1;
    }
    size_t index = HASH_FUNC(key, table->size);
    pthread_mutex_lock(&table->locks[index]);
    HashNode *node = table->buckets[index];
    HashNode *prev = NULL;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            if (prev == NULL) {
                table->buckets[index] = node->next;
            } else {
                prev->next = node->next;
            }
            free(node->key);
            free(node);
            pthread_mutex_unlock(&table->locks[index]);
            return 0;
        }
        prev = node;
        node = node->next;
    }
    pthread_mutex_unlock(&table->locks[index]);
    return -1;
}
