#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "container/graph.h"
#include "lib/unitest.h"

#define NUM_THREADS 4

void *graph_thread_func(void *arg) {
    Graph *graph = (Graph *)arg;
    for (int i = 0; i < 10; i++) {
        int src = rand() % graph->num_vertices;
        int dest = rand() % graph->num_vertices;
        if (graph_add_edge(graph, src, dest) != 0) {
            fprintf(stderr, "Failed to add edge (%d, %d)\n", src, dest);
        } else {
            printf("Added edge (%d, %d)\n", src, dest);
        }
        if (graph_has_edge(graph, src, dest)) {
            printf("Graph has edge (%d, %d)\n", src, dest);
        }
        if (graph_remove_edge(graph, src, dest) != 0) {
            fprintf(stderr, "Failed to remove edge (%d, %d)\n", src, dest);
        } else {
            printf("Removed edge (%d, %d)\n", src, dest);
        }
    }
    return NULL;
}

UNITEST(container_graph_test) {
    int num_vertices = 10;
    Graph *graph = graph_create(num_vertices);
    unitest_assert(graph != NULL, "failed to create graph");
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, graph_thread_func, graph);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    graph_destroy(graph);
}
