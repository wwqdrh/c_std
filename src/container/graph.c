#include <stdlib.h>

#include "container/graph.h"

AdjNode *create_adj_node(int vertex) {
    AdjNode *new_node = malloc(sizeof(AdjNode));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->vertex = vertex;
    new_node->next = NULL;
    return new_node;
}

Graph *graph_create(int num_vertices) {
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->num_vertices = num_vertices;
    graph->adj_lists = calloc(num_vertices, sizeof(AdjList));
    if (graph->adj_lists == NULL) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < num_vertices; i++) {
        pthread_mutex_init(&graph->adj_lists[i].lock, NULL);
    }
    return graph;
}

void graph_destroy(Graph *graph) {
    if (graph == NULL) {
        return;
    }
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode *node = graph->adj_lists[i].head;
        while (node != NULL) {
            AdjNode *next = node->next;
            free(node);
            node = next;
        }
        pthread_mutex_destroy(&graph->adj_lists[i].lock);
    }
    free(graph->adj_lists);
    free(graph);
}

int graph_add_edge(Graph *graph, int src, int dest) {
    if (graph == NULL || src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        return -1;
    }
    AdjNode *new_node = create_adj_node(dest);
    if (new_node == NULL) {
        return -1;
    }
    pthread_mutex_lock(&graph->adj_lists[src].lock);
    new_node->next = graph->adj_lists[src].head;
    graph->adj_lists[src].head = new_node;
    pthread_mutex_unlock(&graph->adj_lists[src].lock);
    return 0;
}

int graph_remove_edge(Graph *graph, int src, int dest) {
    if (graph == NULL || src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        return -1;
    }
    pthread_mutex_lock(&graph->adj_lists[src].lock);
    AdjNode *node = graph->adj_lists[src].head;
    AdjNode *prev = NULL;
    while (node != NULL) {
        if (node->vertex == dest) {
            if (prev == NULL) {
                graph->adj_lists[src].head = node->next;
            } else {
                prev->next = node->next;
            }
            free(node);
            pthread_mutex_unlock(&graph->adj_lists[src].lock);
            return 0;
        }
        prev = node;
        node = node->next;
    }
    pthread_mutex_unlock(&graph->adj_lists[src].lock);
    return -1;
}

int graph_has_edge(Graph *graph, int src, int dest) {
    if (graph == NULL || src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        return -1;
    }
    pthread_mutex_lock(&graph->adj_lists[src].lock);
    AdjNode *node = graph->adj_lists[src].head;
    while (node != NULL) {
        if (node->vertex == dest) {
            pthread_mutex_unlock(&graph->adj_lists[src].lock);
            return 1;
        }
        node = node->next;
    }
    pthread_mutex_unlock(&graph->adj_lists[src].lock);
    return 0;
}
