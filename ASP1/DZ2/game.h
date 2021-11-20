#ifndef GAME_H
#define GAME_H

#include "state.h"
#include <stdbool.h>

StateNode* create_tree(StateNode* initial_state, int dimension, int* all_nums);
StateNode* clear_tree(StateNode* root, int dimension);
void generate_children(StateNode* node, int dimension, int char_sum, int* all_nums);
void print_tree(StateNode* root, int dimension, int char_sum);
void print_solutions(StateNode* root, int dimension, int char_sum);

#endif /* GAME_H */