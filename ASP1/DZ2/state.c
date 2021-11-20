#include <stdlib.h>
#include <stdbool.h>
#include "state.h"
#include "stack.h"
#include "misc.h"

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

// TODO: Implementirati funkciju koja provjerava da li je kvadrat savrsen
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