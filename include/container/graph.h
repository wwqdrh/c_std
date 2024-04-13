#ifndef CONTAINER_GRAPH_H
#define CONTAINER_GRAPH_H

#include <stddef.h>
#include <pthread.h>

typedef struct AdjNode {
    int vertex;
    struct AdjNode *next;
} AdjNode;

typedef struct AdjList {
    AdjNode *head;
    pthread_mutex_t lock;
} AdjList;

typedef struct Graph {
    int num_vertices;
    AdjList *adj_lists;
} Graph;

Graph *graph_create(int num_vertices);
void graph_destroy(Graph *graph);
int graph_add_edge(Graph *graph, int src, int dest);
int graph_remove_edge(Graph *graph, int src, int dest);
int graph_has_edge(Graph *graph, int src, int dest);

#endif
