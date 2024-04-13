#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "container/queue.h"
#include "lib/unitest.h"

#define NUM_THREADS 4

void *producer(void *arg) {
    Queue *queue = (Queue *)arg;
    for (int i = 0; i < 10; i++) {
        int *data = malloc(sizeof(int));
        *data = i;
        if (queue_enqueue(queue, data) != 0) {
            fprintf(stderr, "Failed to enqueue data\n");
            free(data);
        } else {
            printf("Enqueued %d\n", *data);
        }
    }
    return NULL;
}

void *consumer(void *arg) {
    Queue *queue = (Queue *)arg;
    while (!queue_is_empty(queue)) {
        int *data = (int *)queue_dequeue(queue);
        if (data != NULL) {
            printf("Dequeued %d\n", *data);
            free(data);
        }
    }
    return NULL;
}

UNITEST(container_queue_test) {
    Queue *queue = queue_create();
    unitest_assert(queue != NULL, "failed create queue");
    pthread_t producers[NUM_THREADS];
    pthread_t consumers[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&producers[i], NULL, producer, queue);
        pthread_create(&consumers[i], NULL, consumer, queue);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    queue_destroy(queue);
}
