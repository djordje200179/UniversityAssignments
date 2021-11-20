#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHECK_ERROR(pointer) if(pointer == NULL) { printf("MEM_GRESKA\n"); exit(0); }

typedef struct Node {
	char* name;

	struct Node* next;
} Node;

Node* init_node(char* name) {
	Node* node = (Node*)malloc(sizeof(Node));
	CHECK_ERROR(node);

	node->name = name;
	node->next = NULL;

	return node;
}

Node* read_line() {
	char* string = NULL;

	char temp_char;
	int length = 0;

	while((temp_char = getchar()) != '\n') {
		length++;

		string = (char*)realloc(string, sizeof(char) * (length + 1));
		CHECK_ERROR(string);

		string[length - 1] = temp_char;
	}

	if(!string)
		return NULL;
	else {
		string[length] = '\0';

		Node* node = init_node(string);
		return node;
	}
}

Node* read_lines() {
	Node* begin_node = NULL;
	Node* end_node = NULL;

	Node* current_node = NULL;
	while((current_node = read_line()) != NULL) {
		if(!begin_node)
			begin_node = end_node = current_node;
		else {
			end_node->next = current_node;
			end_node = current_node;
		}
	}

	return begin_node;
}

int list_length(Node* list) {
	int counter = 0;
	for(Node* current = list; current; current = current->next)
		counter++;

	return counter;
}

void print_list(Node* list, Node* starred) {
	for(Node* current = list; current; current = current->next)
		printf(current != starred ? "%s\n" : "%s*\n", current->name);
}

Node* remove_node(Node* list, Node* node) {
	Node* next_node = node->next;

	if(list == node) {
		free(node->name);
		free(node);

		list = next_node;
	} else {
		Node* prev_node;
		for(prev_node = list; prev_node->next != node; prev_node = prev_node->next);

		prev_node->next = next_node;

		free(node->name);
		free(node);
	}

	return list;
}

Node* get_node(Node* list, Node* current, int position) {
	for(int i = 0; i < position; i++)
		current = current->next ? current->next : list;
	
	return current;
}

bool process(Node* list, int start, int n) {
	int len = list_length(list);

	if(n < 1 || start >= len)
		return false;

	print_list(list, NULL);

	int step = 0;
	Node* current_node = get_node(list, list, start);
	while(list) {
		printf("STEP%d\n", step++);
		print_list(list, current_node);

		Node* next_node = current_node->next ? current_node->next : list;
		list = remove_node(list, current_node);
		current_node = get_node(list, next_node, n - 1);
	}

	return true;
}

int main() {
	int start, n;
	scanf("%d %d", &start, &n);
	getchar();

	Node* list = read_lines();

	if(list) {
		bool result = process(list, start, n);

		if(!result) {
			printf("GRESKA\n");
			
			while(list)
				list = remove_node(list, list);
		}
	}
	
	return 0;
}
