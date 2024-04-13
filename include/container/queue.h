#ifndef CONTAINER_QUEUE_H
#define CONTAINER_QUEUE_H

#include <stddef.h>
#include <pthread.h>

typedef struct QueueNode {
    void *data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
    size_t size;
    pthread_mutex_t lock;
} Queue;

Queue *queue_create();
void queue_destroy(Queue *queue);
int queue_enqueue(Queue *queue, void *data);
void *queue_dequeue(Queue *queue);
size_t queue_size(Queue *queue);
int queue_is_empty(Queue *queue);

#endif
