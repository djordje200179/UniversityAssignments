#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define EMPTY_FIELD 0

struct Stack;
typedef struct StateNode {
	int** values;
	bool* used_nums;
	int level;

	struct Stack* children;
} StateNode;

typedef struct StackNode {
	struct StateNode* info;

	struct StackNode* next;
	struct StackNode* prev;
} StackNode;

typedef struct Stack {
	StackNode* head;
	StackNode* tail;
} Stack;

int calc_char_sum(int nums[], int dimension) {
	int n = dimension * dimension;
	int sum = 0;
	for(int i = 0; i < n; i++)
		sum += nums[i];

	return sum / dimension;
}

int* allocate_int_array(int dimension) {
	int* array = (int*)calloc(dimension, sizeof(int));

	return array;
}

bool* allocate_bool_array(int dimension) {
	bool* array = (bool*)calloc(dimension, sizeof(bool));

	return array;
}

int* clone_int_array(int* array, int dimension) {
	int* new_array = allocate_int_array(dimension);
	memcpy(new_array, array, dimension * sizeof(int));

	return new_array;
}

bool* clone_bool_array(bool* array, int dimension) {
	bool* new_array = allocate_bool_array(dimension);
	memcpy(new_array, array, dimension * sizeof(bool));

	return new_array;
}

int** allocate_matrix(int dimension) {
	int** matrix = (int**)calloc(dimension, sizeof(int*));

	if(!matrix)
		return NULL;

	for(int i = 0; i < dimension; i++) {
		matrix[i] = (int*)calloc(dimension, sizeof(int));

		if(!matrix[i])
			return NULL;
	}

	return matrix;
}

int** deallocate_matrix(int** matrix, int dimension) {
	for(int i = 0; i < dimension; i++)
		free(matrix[i]);

	free(matrix);

	return NULL;
}

StackNode* init_stack_node(StateNode* data) {
	StackNode* stack_node = (StackNode*)malloc(sizeof(StackNode));

	if(!stack_node)
		return NULL;

	stack_node->info = data;
	stack_node->next = stack_node->prev = NULL;

	return stack_node;
}

Stack* init_stack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));

	if(!stack)
		return NULL;

	stack->head = stack->tail = NULL;

	return stack;
}

StateNode* stack_pop(Stack* stack) {
	StackNode* node = stack->tail;

	if(!node)
		return NULL;

	StateNode* data = node->info;
	StackNode* prev_node = node->prev;

	if(prev_node) {
		stack->tail = prev_node;
		prev_node->next = NULL;
	} else
		stack->head = stack->tail = NULL;

	free(node);

	return data;
}

void stack_push(Stack* stack, StateNode* data) {
	StackNode* node = init_stack_node(data);
	StackNode* prev_node = stack->tail;

	if(prev_node) {
		prev_node->next = node;
		node->prev = prev_node;

		stack->tail = node;
	} else
		stack->head = stack->tail = node;
}

bool stack_is_empty(Stack* stack) {
	return stack->head == NULL;
}

StateNode* init_state_node(int dimension) {
	StateNode* state = (StateNode*)malloc(sizeof(StateNode));

	if(!state)
		return NULL;

	state->children = init_stack();
	state->values = allocate_matrix(dimension);
	state->used_nums = allocate_bool_array(dimension * dimension);
	state->level = 0;

	return state;
}

StateNode* clone_state_node(StateNode* source_state, int dimension) {
	StateNode* new_state = init_state_node(dimension);

	for(int i = 0; i < dimension; i++)
		for(int j = 0; j < dimension; j++)
			new_state->values[i][j] = source_state->values[i][j];

	for(int i = 0; i < dimension * dimension; i++)
		new_state->used_nums[i] = source_state->used_nums[i];

	return new_state;
}

StateNode* clear_state_node(StateNode* state, int dimension) {
	deallocate_matrix(state->values, dimension);
	free(state->used_nums);
	free(state);

	return NULL;
}

bool is_state_plausible(StateNode* state, int dimension, int char_sum) {
	int temp_sum = 0;
	bool has_zero = false;
	for(int i = 0; i < dimension; i++) {
		temp_sum = temp_sum + state->values[i][i];

		if(state->values[i][i] == EMPTY_FIELD)
			has_zero = true;
	}
	if(!has_zero) {
		if(temp_sum != char_sum)
			return false;
	} else {
		if(temp_sum >= char_sum)
			return false;
	}

	temp_sum = 0;
	has_zero = false;
	for(int i = 0; i < dimension; i++) {
		temp_sum = temp_sum + state->values[i][dimension - 1 - i];

		if(state->values[i][dimension - 1 - i] == EMPTY_FIELD)
			has_zero = true;
	}
	if(!has_zero) {
		if(temp_sum != char_sum)
			return false;
	} else {
		if(temp_sum >= char_sum)
			return false;
	}

	for(int i = 0; i < dimension; i++) {
		temp_sum = 0;
		has_zero = false;

		for(int j = 0; j < dimension; j++) {
			temp_sum += state->values[i][j];

			if(state->values[i][j] == EMPTY_FIELD)
				has_zero = true;
		}

		if(!has_zero) {
			if(temp_sum != char_sum)
				return false;
		} else {
			if(temp_sum >= char_sum)
				return false;
		}
	}

	for(int i = 0; i < dimension; i++) {
		temp_sum = 0;
		has_zero = false;

		for(int j = 0; j < dimension; j++) {
			temp_sum += state->values[j][i];

			if(state->values[j][i] == EMPTY_FIELD)
				has_zero = true;
		}

		if(!has_zero) {
			if(temp_sum != char_sum)
				return false;
		} else {
			if(temp_sum >= char_sum)
				return false;
		}
	}

	return true;
}

bool is_state_magic(StateNode* state, int dimension, int char_sum) {
	for(int i = 0; i < dimension; i++)
		for(int j = 0; j < dimension; j++)
			if(state->values[i][j] == EMPTY_FIELD)
				return false;

	int temp_sum = 0;
	for(int i = 0; i < dimension; i++)
		temp_sum = temp_sum + state->values[i][i];
	if(temp_sum != char_sum)
		return false;

	temp_sum = 0;
	for(int i = 0; i < dimension; i++)
		temp_sum = temp_sum + state->values[i][dimension - 1 - i];
	if(temp_sum != char_sum)
		return false;

	for(int i = 0; i < dimension; i++) {
		temp_sum = 0;
		for(int j = 0; j < dimension; j++)
			temp_sum += state->values[i][j];

		if(temp_sum != char_sum)
			return false;
	}

	for(int i = 0; i < dimension; i++) {
		temp_sum = 0;
		for(int j = 0; j < dimension; j++)
			temp_sum += state->values[j][i];

		if(temp_sum != char_sum)
			return false;
	}

	return true;
}

bool is_state_special(StateNode* state, int dimension, int char_sum) {
	if(!is_state_magic(state, dimension, char_sum))
		return false;

	int temp_sum = 0;
	for(int i = 1; i < dimension; i++) {
		temp_sum = 0;

		for(int j = 0; j < dimension; j++)
			temp_sum += state->values[(i + j) % dimension][j];

		if(temp_sum != char_sum)
			return false;
	}

	for(int i = 1; i < dimension; i++) {
		temp_sum = 0;

		for(int j = 0; j < dimension; j++)
			temp_sum += state->values[(i + j) % dimension][dimension - 1 - j];

		if(temp_sum != char_sum)
			return false;
	}

	return true;
}

void generate_children(StateNode* node, int dimension, int char_sum, int* all_nums) {
	int empty_i = 0, empty_j = 0;
	for(empty_i = 0; empty_i < dimension; empty_i++)
		for(empty_j = 0; empty_j < dimension; empty_j++)
			if(node->values[empty_i][empty_j] == EMPTY_FIELD)
				goto end_search_for_empty;

	if(empty_i == dimension || empty_j == dimension)
		return;

end_search_for_empty:
	for(int k = 0; k < dimension * dimension; k++) {
		if(node->used_nums[k])
			continue;

		StateNode* new_state = clone_state_node(node, dimension);
		new_state->level = node->level + 1;
		new_state->values[empty_i][empty_j] = all_nums[k];
		new_state->used_nums[k] = true;

		if(is_state_plausible(new_state, dimension, char_sum))
			stack_push(node->children, new_state);
		else
			new_state = clear_state_node(new_state, dimension);
	}
}

StateNode* create_tree(StateNode* initial_state, int dimension, int* all_nums) {
	int char_sum = calc_char_sum(all_nums, dimension);
	for(int i = 0; i < dimension; i++)
		for(int j = 0; j < dimension; j++)
			if(initial_state->values[i][j] != EMPTY_FIELD)
				initial_state->used_nums[i * dimension + j] = true;

	if(!is_state_plausible(initial_state, dimension, char_sum))
		return NULL;

	Stack* traversal_stack = init_stack();
	stack_push(traversal_stack, initial_state);

	while(!stack_is_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		generate_children(node, dimension, char_sum, all_nums);

		for(StackNode* child = node->children->tail; child != NULL; child = child->prev)
			stack_push(traversal_stack, child->info);
	}

	return initial_state;
}

StateNode* clear_tree(StateNode* root, int dimension) {
	Stack* traversal_stack = init_stack();
	stack_push(traversal_stack, root);

	while(!stack_is_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		for(StackNode* child = node->children->tail; child; child = child->prev)
			stack_push(traversal_stack, child->info);

		clear_state_node(node, dimension);
	}

	return NULL;
}

void print_tree(StateNode* root, int dimension, int char_sum) {
	Stack* traversal_stack = init_stack();
	stack_push(traversal_stack, root);

	while(!stack_is_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		if(stack_is_empty(node->children)) {
			if(is_state_special(node, dimension, 15))
				printf("\033[0;36m");
			else if(is_state_magic(node, dimension, 15))
				printf("\033[0;32m");
			else
				printf("\033[0;31m");
		} else
			printf("\033[0;33m");

		for(int i = 0; i < dimension; i++) {
			for(int j = 0; j < node->level; j++)
				printf("\t");

			for(int j = 0; j < dimension; j++)
				printf("%d ", node->values[i][j]);
			printf("\n");
		}
		printf("\n");
		printf("\033[0m");

		for(StackNode* child = node->children->tail; child != NULL; child = child->prev)
			stack_push(traversal_stack, child->info);
	}
}

void print_solutions(StateNode* root, int dimension, int char_sum) {
	Stack* traversal_stack = init_stack();
	stack_push(traversal_stack, root);

	while(!stack_is_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		if(stack_is_empty(node->children) && is_state_magic(node, dimension, char_sum)) {
			if(is_state_special(node, dimension, char_sum)) {
				printf("\033[0;36m");
				printf("Savrsen kvadrat\n");
				printf("\033[0;33m");
			} else {
				printf("\033[0;32m");
				printf("Magican kvadrat\n");
				printf("\033[0;33m");
			}

			for(int i = 0; i < dimension; i++) {
				for(int j = 0; j < dimension; j++)
					printf("%d ", node->values[i][j]);

				printf("\n");
			}
			printf("\033[0m");

			printf("\n");
		}

		for(StackNode* child = node->children->tail; child != NULL; child = child->prev)
			stack_push(traversal_stack, child->info);
	}
}

int main() {
	int dimension = 3;
	int choice = 1;
	StateNode* initial_state = NULL;
	StateNode* tree = NULL;
	int* all_nums = NULL;

	while(true) {
		printf("Odaberite operaciju:\n");
		printf("\t 1) Unos informacija\n");
		printf("\t 2) Generisanje stabla\n");
		printf("\t 3) Ispis kompletnog stabla\n");
		printf("\t 4) Ispis rjesenja\n");
		printf("\t 0) Izlazak iz programa\n");
		scanf("%d", &choice);

		switch(choice) {
		case 1:
			printf("Unesite dimenzije kvadrata: ");
			scanf("%d", &dimension);

			initial_state = init_state_node(dimension);
			all_nums = allocate_int_array(dimension * dimension);

			printf("Popunite kvadrat podacima (u prazna mjesta upisite 0): \n");
			for(int i = 0; i < dimension; i++)
				for(int j = 0; j < dimension; j++)
					scanf("%d", &initial_state->values[i][j]);
			printf("\n");

			printf("Unesite preostale brojeve: ");
			for(int i = 0; i < dimension; i++)
				for(int j = 0; j < dimension; j++)
					if(initial_state->values[i][j] != EMPTY_FIELD)
						all_nums[i * dimension + j] = initial_state->values[i][j];
					else
						scanf("%d", &all_nums[i * dimension + j]);
			printf("\n");

			break;
		case 2:
			tree = create_tree(initial_state, dimension, all_nums);

			break;
		case 3:
			if(!tree)
				printf("Pocetno stanje nije validno ili stablo nije generisano\n");
			else
				print_tree(tree, dimension, calc_char_sum(all_nums, dimension));

			break;
		case 4:
			if(!tree)
				printf("Pocetno stanje nije validno ili stablo nije generisano\n");
			else
				print_solutions(tree, dimension, calc_char_sum(all_nums, dimension));

			break;
		case 0:
			clear_tree(initial_state, dimension);
			free(all_nums);

			return 0;
		}
	}
}