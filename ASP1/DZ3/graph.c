#include "graph.h"
#include "deque.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

Graph* graph_init(int size, char names[]) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));

	if(!graph)
		return NULL;

	graph->size = size;

	graph->connections = (bool**)calloc(size, sizeof(bool*));
	for(int i = 0; i < size; i++)
		graph->connections[i] = (bool*)calloc(size, sizeof(bool));

	graph->names = (char*)calloc(size, sizeof(char));
	memcpy(graph->names, names, graph->size * sizeof(char));

	return graph;
}

void graph_destroy(Graph* graph) {
	for(int i = 0; i < graph->size; i++)
		free(graph->connections[i]);
	free(graph->connections);

	free(graph->names);

	free(graph);
}

static int graph_find_index(Graph* graph, char name) {
	int index = 0;
	while(graph->names[index] != name)
		index++;

	return index;
}

void graph_add_vertex(Graph* graph, char name) {
	graph->size++;

	graph->connections = (bool**)realloc(graph->connections, graph->size * sizeof(bool*));
	graph->connections[graph->size - 1] = (bool*)calloc(graph->size, sizeof(bool));
	for(int i = 0; i < graph->size - 1; i++)
		graph->connections[i] = (bool*)realloc(graph->connections[i], graph->size * sizeof(bool));

	graph->names = (char*)realloc(graph->names, graph->size * sizeof(char));

	graph->names[graph->size - 1] = name;
}

void graph_remove_vertex(Graph* graph, char name) {
	int index = graph_find_index(graph, name);

	for(int i = 0; i < graph->size; i++) {
		for(int j = index; j < graph->size - 1; j++)
			graph->connections[i][j] = graph->connections[i][j + 1];

		graph->connections[i] = (bool*)realloc(graph->connections[i], (graph->size - 1) * sizeof(bool));
	}

	for(int i = index; i < graph->size - 1; i++)
		graph->connections[i] = graph->connections[i + 1];

	free(graph->connections[graph->size - 1]);
	graph->connections = (bool**)realloc(graph->connections, (graph->size - 1) * sizeof(bool*));

	for(int i = index; i < graph->size - 1; i++)
		graph->names[i] = graph->names[i + 1];

	graph->names = (char*)realloc(graph->names, (graph->size - 1) * sizeof(char));

	graph->size--;
}

void graph_add_edge(Graph* graph, char start_vertex, char end_vertex) {
	int start_index = graph_find_index(graph, start_vertex);
	int end_index = graph_find_index(graph, end_vertex);

	graph->connections[start_index][end_index] = true;
}

void graph_remove_edge(Graph* graph, char start_vertex, char end_vertex) {
	int start_index = graph_find_index(graph, start_vertex);
	int end_index = graph_find_index(graph, end_vertex);

	graph->connections[start_vertex][end_vertex] = false;
}

int graph_shortest_path(Graph* graph, char start_vertex, char end_vertex) {
	int start_index = graph_find_index(graph, start_vertex);
	int end_index = graph_find_index(graph, end_vertex);

	int* distances = (int*)calloc(graph->size, sizeof(int));
	for(int i = 0; i < graph->size; i++)
		distances[i] = INT_MAX;
	distances[start_index] = 0;

	bool* visited = (bool*)calloc(graph->size, sizeof(bool));
	visited[start_index] = true;

	Deque* deque = deque_init();
	deque_push_back(deque, start_index);
	while(!deque_empty(deque)) {
		int current_index = deque_pop_front(deque);

		for(int next_index = 0; next_index < graph->size; next_index++) {
			if(!graph->connections[current_index][next_index])
				continue;

			if(!visited[next_index]) {
				visited[next_index] = true;
				distances[next_index] = distances[current_index] + 1;

				if(next_index == end_index)
					return distances[end_index];

				deque_push_back(deque, next_index);
			}
		}
	}

	return -1;
}

int graph_shortest_special_path(Graph* graph, char start_vertex, char end_vertex) {
	int start_index = graph_find_index(graph, start_vertex);
	int end_index = graph_find_index(graph, end_vertex);

	int* distances = (int*)calloc(graph->size, sizeof(int));
	for(int i = 0; i < graph->size; i++)
		distances[i] = INT_MAX;
	distances[start_index] = 0;

	int* visited = (int*)calloc(graph->size, sizeof(int));
	visited[start_index] = 1;

	Deque* traversal_deque = deque_init();
	Deque* move_deque = deque_init();

	deque_push_back(traversal_deque, start_index);
	deque_push_back(move_deque, 1);
	while(!deque_empty(traversal_deque)) {
		int current_index = deque_pop_front(traversal_deque);
		int current_move = deque_pop_front(move_deque);

		for(int next_index = 0; next_index < graph->size; next_index++) {
			if(!graph->connections[current_index][next_index])
				continue;

			visited[next_index]++;

			if(current_move == 1) {
				distances[next_index] = distances[current_index] + 1;

				if(next_index == end_index)
					return distances[end_index];

				deque_push_back(traversal_deque, next_index);
				deque_push_back(move_deque, 2);
			} else {
				for(int next_next_index = 0; next_next_index < graph->size; next_next_index++) {
					if(!graph->connections[next_index][next_next_index] || visited[next_next_index] >= 2)
						continue;

					visited[next_next_index]++;

					distances[next_next_index] = distances[current_index] + 2;

					if(next_next_index == end_index)
						return distances[end_index];

					deque_push_back(traversal_deque, next_next_index);
					deque_push_back(move_deque, 1);
				}
			}
		}
	}

	return -1;
}

static void graph_print_path(Graph* graph, Deque* path_index_deque, PathType path_type) {
	int n = path_index_deque->size;
	if(path_type == NORMAL) {
		for(int i = 0; i < n; i++) {
			int index = deque_pop_front(path_index_deque);
			char name = graph->names[index];
			deque_push_back(path_index_deque, index);

			printf("%c", name);
			printf(i < n - 1 ? "->" : "\n", name);
		}
	} else {
		for(int i = 0, move_count = 0; i < n; i++, move_count++) {
			int index1 = deque_pop_front(path_index_deque);
			char name1 = graph->names[index1];
			deque_push_back(path_index_deque, index1);

			if(move_count % 2 == 1 || i == 0)
				printf("%c", name1);
			else {
				int index2 = deque_pop_front(path_index_deque);
				char name2 = graph->names[index2];
				deque_push_back(path_index_deque, index2);

				i++;

				printf("(%c->%c)", name1, name2);
			}

			printf(i < n - 1 ? "->" : "\n");
		}
	}
}

void graph_print_paths(Graph* graph, char start_vertex, char end_vertex, int distance, PathType path_type) {
	int start_index = graph_find_index(graph, start_vertex);
	int end_index = graph_find_index(graph, end_vertex);

	Deque* traversal_index_deque = deque_init();
	Deque* traversal_level_deque = deque_init();
	Deque* path_index_deque = deque_init();
	if(!traversal_index_deque || !traversal_level_deque || !path_index_deque)
		return;

	deque_push_back(traversal_index_deque, start_index);
	deque_push_back(traversal_level_deque, 0);
	while(!deque_empty(traversal_index_deque)) {
		int current_index = deque_pop_back(traversal_index_deque);
		int current_level = deque_pop_back(traversal_level_deque);

		deque_push_back(path_index_deque, current_index);

		if(current_index == end_index && current_level == distance)
			graph_print_path(graph, path_index_deque, path_type);
		else
			for(int next_index = 0; next_index < graph->size; next_index++) {
				if(!graph->connections[current_index][next_index])
					continue;

				if(current_level + 1 == distance && next_index != end_index)
					continue;

				deque_push_back(traversal_index_deque, next_index);
				deque_push_back(traversal_level_deque, current_level + 1);
			}

		while(path_index_deque->size - 1 >= deque_peek_back(traversal_level_deque))
			deque_pop_back(path_index_deque);
	}
}

void graph_print(Graph* graph) {
	printf("# ");
	for(int i = 0; i < graph->size; i++)
		printf("%c ", graph->names[i]);
	printf("\n");

	for(int i = 0; i < graph->size; i++) {
		printf("%c ", graph->names[i]);

		for(int j = 0; j < graph->size; j++)
			printf("%d ", graph->connections[i][j]);

		printf("\n");
	}
}