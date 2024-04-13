#include "container/queue.h"
#include <stdlib.h>

Queue *queue_create() {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    pthread_mutex_init(&queue->lock, NULL);
    return queue;
}

void queue_destroy(Queue *queue) {
    if (queue == NULL) {
        return;
    }
    QueueNode *current = queue->head;
    while (current != NULL) {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }
    pthread_mutex_destroy(&queue->lock);
    free(queue);
}

int queue_enqueue(Queue *queue, void *data) {
    if (queue == NULL) {
        return -1;
    }
    QueueNode *node = malloc(sizeof(QueueNode));
    if (node == NULL) {
        return -1;
    }
    node->data = data;
    node->next = NULL;
    pthread_mutex_lock(&queue->lock);
    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    pthread_mutex_unlock(&queue->lock);
    return 0;
}

void *queue_dequeue(Queue *queue) {
    if (queue == NULL || queue->head == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&queue->lock);
    QueueNode *node = queue->head;
    void *data = node->data;
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    queue->size--;
    pthread_mutex_unlock(&queue->lock);
    free(node);
    return data;
}

size_t queue_size(Queue *queue) {
    if (queue == NULL) {
        return 0;
    }
    pthread_mutex_lock(&queue->lock);
    size_t size = queue->size;
    pthread_mutex_unlock(&queue->lock);
    return size;
}

int queue_is_empty(Queue *queue) {
    if (queue == NULL) {
        return 1;
    }
    pthread_mutex_lock(&queue->lock);
    int is_empty = (queue->size == 0);
    pthread_mutex_unlock(&queue->lock);
    return is_empty;
}
