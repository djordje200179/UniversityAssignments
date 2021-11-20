#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "deque.h"

typedef struct Graph {
	int size;

	bool** connections;
	char* names;
} Graph;

typedef enum PathType {
	NORMAL, SPECIAL
} PathType;

Graph* graph_init(int size, char names[]);
void graph_destroy(Graph* graph);

void graph_add_vertex(Graph* graph, char name);
void graph_remove_vertex(Graph* graph, char name);

void graph_add_edge(Graph* graph, char start_vertex, char end_vertex);
void graph_remove_edge(Graph* graph, char start_vertex, char end_vertex);

int graph_shortest_path(Graph* graph, char start_vertex, char end_vertex);
int graph_shortest_special_path(Graph* graph, char start_vertex, char end_vertex);
void graph_print_paths(Graph* graph, char start_vertex, char end_vertex, int distance, PathType path_type);

void graph_print(Graph* graph);

#endif /* GRAPH_H */