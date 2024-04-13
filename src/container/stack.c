#include <stdlib.h>

#include "container/stack.h"

Stack *stack_create() {
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;
    pthread_mutex_init(&stack->lock, NULL);
    return stack;
}

void stack_destroy(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }
    pthread_mutex_destroy(&stack->lock);
    free(stack);
}

int stack_push(Stack *stack, void *data) {
    if (stack == NULL) {
        return -1;
    }
    StackNode *node = malloc(sizeof(StackNode));
    if (node == NULL) {
        return -1;
    }
    node->data = data;
    pthread_mutex_lock(&stack->lock);
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    pthread_mutex_unlock(&stack->lock);
    return 0;
}

void *stack_pop(Stack *stack) {
    if (stack == NULL || stack->top == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&stack->lock);
    StackNode *node = stack->top;
    void *data = node->data;
    stack->top = node->next;
    stack->size--;
    pthread_mutex_unlock(&stack->lock);
    free(node);
    return data;
}

size_t stack_size(Stack *stack) {
    if (stack == NULL) {
        return 0;
    }
    pthread_mutex_lock(&stack->lock);
    size_t size = stack->size;
    pthread_mutex_unlock(&stack->lock);
    return size;
}

int stack_is_empty(Stack *stack) {
    if (stack == NULL) {
        return 1;
    }
    pthread_mutex_lock(&stack->lock);
    int is_empty = (stack->size == 0);
    pthread_mutex_unlock(&stack->lock);
    return is_empty;
}
