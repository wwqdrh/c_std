#include <stdlib.h>
#include <pthread.h>

#include "container/stack.h"
#include "lib/unitest.h"

#define NUM_THREADS 4

void *pusher(void *arg) {
    Stack *stack = (Stack *)arg;
    for (int i = 0; i < 10; i++) {
        int *data = malloc(sizeof(int));
        *data = i;
        if (stack_push(stack, data) != 0) {
            fprintf(stderr, "Failed to push data\n");
            free(data);
        } else {
            printf("Pushed %d\n", *data);
        }
    }
    return NULL;
}

void *popper(void *arg) {
    Stack *stack = (Stack *)arg;
    while (!stack_is_empty(stack)) {
        int *data = (int *)stack_pop(stack);
        if (data != NULL) {
            printf("Popped %d\n", *data);
            free(data);
        }
    }
    return NULL;
}

UNITEST(container_stack_test) {
    Stack *stack = stack_create();
    unitest_assert(stack != NULL, "failed to create stack"); 

    pthread_t pushers[NUM_THREADS];
    pthread_t poppers[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&pushers[i], NULL, pusher, stack);
        pthread_create(&poppers[i], NULL, popper, stack);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(pushers[i], NULL);
        pthread_join(poppers[i], NULL);
    }

    stack_destroy(stack);
}
