#include "game.h"
#include "misc.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

#define YELLOW_COLOR "\033[0;33m"
#define GREEN_COLOR "\033[0;32m"
#define BLUE_COLOR "\033[0;34m"
#define RED_COLOR "\033[0;31m"
#define NO_COLOR "\033[0m"

inline void set_text_color(char* color) {
	printf(color);
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

	while(!stack_empty(traversal_stack)) {
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

	while(!stack_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		for(StackNode* child = node->children->tail; child; child = child->prev)
			stack_push(traversal_stack, child->info);

		clear_state_node(node, dimension);
	}

	return NULL;
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

void print_tree(StateNode* root, int dimension, int char_sum) {
	Stack* traversal_stack = init_stack();
	stack_push(traversal_stack, root);

	while(!stack_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		if(stack_empty(node->children)) {
			if(is_state_special(node, dimension, char_sum))
				set_text_color(BLUE_COLOR);
			else if(is_state_magic(node, dimension, char_sum))
				set_text_color(GREEN_COLOR);
			else
				set_text_color(RED_COLOR);
		} else
			set_text_color(YELLOW_COLOR);

		for(int i = 0; i < dimension; i++) {
			for(int j = 0; j < node->level; j++)
				printf("\t");

			for(int j = 0; j < dimension; j++)
				printf("%d ", node->values[i][j]);
			printf("\n");
		}
		printf("\n");
		set_text_color(NO_COLOR);

		for(StackNode* child = node->children->tail; child != NULL; child = child->prev)
			stack_push(traversal_stack, child->info);
	}
}

void print_solutions(StateNode* root, int dimension, int char_sum) {
	Stack* traversal_stack = init_stack();
	stack_push(traversal_stack, root);

	while(!stack_empty(traversal_stack)) {
		StateNode* node = stack_pop(traversal_stack);

		if(stack_empty(node->children) && is_state_magic(node, dimension, char_sum)) {
			if(is_state_special(node, dimension, char_sum)) {
				set_text_color(BLUE_COLOR);
				printf("Savrsen kvadrat\n");
				set_text_color(NO_COLOR);
			} else {
				set_text_color(GREEN_COLOR);
				printf("Magican kvadrat\n");
				set_text_color(NO_COLOR);
			}

			set_text_color(YELLOW_COLOR);
			for(int i = 0; i < dimension; i++) {
				for(int j = 0; j < dimension; j++)
					printf("%d ", node->values[i][j]);

				printf("\n");
			}
			printf("\n");

			set_text_color(NO_COLOR);
		}

		for(StackNode* child = node->children->tail; child != NULL; child = child->prev)
			stack_push(traversal_stack, child->info);
	}
}