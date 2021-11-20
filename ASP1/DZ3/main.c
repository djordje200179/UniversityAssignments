#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void maja(Graph* graph, char start, char end) {
	int distance = graph_shortest_path(graph, start, end);

	if(distance == -1)
		printf("Maja ne moze doci do cilja!\n");
	else {
		printf("---------Maja---------\n");
		printf("Broj poteza: %d\n", distance);
		printf("Putevi:\n");
		graph_print_paths(graph, start, end, distance, NORMAL);
	}
}

void sanja(Graph* graph, char start, char end) {
	int distance = graph_shortest_special_path(graph, start, end);

	if(distance == -1)
		printf("Sanja ne moze doci do cilja!\n");
	else {
		int temp_distance = distance;
		int moves = 0;
		while(temp_distance) {
			temp_distance -= moves % 2 == 0 ? 1 : 2;
			moves++;
		}

		printf("---------Sanja--------\n");
		printf("Broj poteza: %d\n", moves);
		printf("Putevi:\n");
		graph_print_paths(graph, start, end, distance, SPECIAL);
	}
}

int main() {
	int v;
	printf("Broj cvorova: ");
	scanf("%d", &v);

	char* names = (char*)calloc(v, sizeof(char));
	printf("Imena cvorova: ");
	for(int i = 0; i < v; i++)
		scanf(" %c", &names[i]);

	Graph* graph = graph_init(v, names);
	free(names);

	char start, end;
	printf("Pocetni i krajnji cvor: ");
	scanf(" %c %c", &start, &end);

	int e;
	printf("Broj grana: ");
	scanf("%d", &e);

	printf("Grane:");
	printf("\n");
	for(int i = 0; i < e; i++) {
		char a, b;
		scanf(" %c %c", &a, &b);

		graph_add_edge(graph, a, b);
	}
	printf("\n");

	printf("-----Izgled grafa-----\n");
	graph_print(graph);
	printf("\n");

	printf("----Rezultati igre----\n");
	maja(graph, start, end);
	sanja(graph, start, end);

	graph_destroy(graph);

	return 0;
}