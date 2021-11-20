#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define LINE_SIZE 500
#define STRING_SIZE 256
#define EARTH_RADIUS 6371

#define CHECK_POINTER(pointer) if(pointer == NULL) { printf("MEM_GRESKA"); exit(0); }

typedef struct Stop {
	int id;
	char name[STRING_SIZE];
	double latitude;
	double longitude;
	int zone;
} Stop;

typedef struct Result {
	char name[STRING_SIZE];
	double distance;
	int zone;
} Result;

typedef struct Node {
	void* info;

	struct Node* next;
	struct Node* prev;
} Node;

typedef struct List {
	Node* start;
	Node* end;
} List;

Stop* init_stop(int id, char name[], double latitude, double longitude, int zone) {
	Stop* stop = (Stop*)malloc(sizeof(Stop));
	CHECK_POINTER(stop);

	stop->id = id;
	strcpy(stop->name, name);
	stop->latitude = latitude;
	stop->longitude = longitude;
	stop->zone = zone;

	return stop;
}

Result* init_result(char name[], double distance, int zone) {
	Result* result = (Result*)malloc(sizeof(Result));
	CHECK_POINTER(result);

	strcpy(result->name, name);
	result->distance = distance;
	result->zone = zone;

	return result;
}

Node* init_node(void* info) {
	Node* node = (Node*)malloc(sizeof(Node));
	CHECK_POINTER(node);

	node->info = info;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

List* init_list() {
	List* list = (List*)malloc(sizeof(List));
	CHECK_POINTER(list);

	list->start = NULL;
	list->end = NULL;

	return list;
}

void list_append(List* list, Node* node) {
	if(list->start == NULL)
		list->start = list->end = node;
	else {
		Node* last_node = list->end;

		last_node->next = node;
		node->prev = last_node;

		list->end = node;
	}
}

double deg_to_rad(double degrees) {
	return degrees * 3.14 / 180;
}

double calculate_distance(Stop* a, Stop* b) {
	double lat1 = deg_to_rad(a->latitude);
	double lat2 = deg_to_rad(b->latitude);
	double lon1 = deg_to_rad(a->longitude);
	double lon2 = deg_to_rad(b->longitude);
	double t1 = pow(sin((lat1 - lat2) / 2), 2);
	double t2 = pow(sin((lon1 - lon2) / 2), 2);

	double d = 2 * EARTH_RADIUS * asin(sqrt(t1 + t2 * cos(lat1) * cos(lat2)));

	return d;
}

Stop* read_file_line(FILE* file) {
	char line[LINE_SIZE];

	if(fgets(line, LINE_SIZE, file) == NULL)
		return NULL;

	int id;
	char name[STRING_SIZE];
	double latitude;
	double longitude;
	int zone;

	sscanf(line, "%d ! %[^!] ! %lf ! %lf ! %d \n", &id, name, &latitude, &longitude, &zone);

	Stop* stop = init_stop(id, name, latitude, longitude, zone);
	return stop;
}

List* read_file(char line[], char dir) {
	char input_file_name[30];
	sprintf(input_file_name, "%s_dir%c.txt", line, dir);

	FILE* input_file = fopen(input_file_name, "r");
	if(!input_file)
		return NULL;

	List* list = init_list();

	while(1) {
		Stop* stop = read_file_line(input_file);

		if(!stop)
			break;
		else
			list_append(list, init_node(stop));
	}

	fclose(input_file);

	return list;
}

List* get_same_stops(List* stops_a, List* stops_b) {
	List* list = init_list();

	for(Node* node_a = stops_a->start; node_a; node_a = node_a->next) {
		Stop* stop_a = (Stop*)node_a->info;

		for(Node* node_b = stops_b->start; node_b; node_b = node_b->next) {
			Stop* stop_b = (Stop*)node_b->info;

			if(strcmp(stop_a->name, stop_b->name) == 0) {
				double distance = calculate_distance(stop_a, stop_b);

				Result* result = init_result(stop_a->name, distance, stop_a->zone);
				
				list_append(list, init_node(result));
			}
		}
	}

	return list;
}

bool write_file(List* list, char line[]) {
	char input_file_name[30];
	sprintf(input_file_name, "%s_station.txt", line);

	FILE* output_file = fopen(input_file_name, "w");
	if(!output_file)
		return false;

	for(Node* node_i = list->start; node_i; node_i = node_i->next)
		for(Node* node_j = node_i->next; node_j; node_j = node_j->next) {
			Result* result_i = (Result*)node_i->info;
			Result* result_j = (Result*)node_j->info;

			if(strcmp(result_i->name, result_j->name) > 0) {
				Result* temp_stop = result_i;
				node_i->info = result_j;
				node_j->info = temp_stop;
			}
		}

	for(Node* current_node = list->start; current_node; current_node = current_node->next) {
		Result* result = (Result*)current_node->info;

		fprintf(output_file, "%s!%.2lf!%d\n", result->name, result->distance, result->zone);
	}

	fclose(output_file);
}

void free_list(List* list) {
	Node* current_node = list->start;
	while(current_node) {
		Node* temp_node = current_node->next;

		free(current_node->info);
		free(current_node);

		current_node = temp_node;
	}

	free(list);
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("ARG_GRESKA");

		return 0;
	}

	for(int i = 1; i < argc; i++) {
		char* line= argv[i];
		
		List* stops_a = read_file(line, 'A');

		if(!stops_a) {
			printf("DAT_GRESKA");
			
			exit(0);
		}

		List* stops_b = read_file(line, 'B');

		if(!stops_b) {
			printf("DAT_GRESKA");

			free_list(stops_a);

			exit(0);
		}

		List* same_stops = get_same_stops(stops_a, stops_b);
		bool result = write_file(same_stops, line);

		if(!result)
			printf("DAT_GRESKA");
		
		free_list(stops_a);
		free_list(stops_b);
		free_list(same_stops);
	}

	return 0;
}