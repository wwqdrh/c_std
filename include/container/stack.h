#ifndef CONTAINER_STACK_H
#define CONTAINER_STACK_H

#include <stddef.h>
#include <pthread.h>

typedef struct StackNode {
    void *data;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *top;
    size_t size;
    pthread_mutex_t lock;
} Stack;

Stack *stack_create();
void stack_destroy(Stack *stack);
int stack_push(Stack *stack, void *data);
void *stack_pop(Stack *stack);
size_t stack_size(Stack *stack);
int stack_is_empty(Stack *stack);

#endif
