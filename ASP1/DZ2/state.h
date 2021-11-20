#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

#define EMPTY_FIELD 0

struct Stack;
typedef struct StateNode {
	int** values;
	bool* used_nums;
	int level;

	struct Stack* children;
} StateNode;

StateNode* init_state_node(int dimension);
StateNode* clone_state_node(StateNode* source, int dimension);
StateNode* clear_state_node(StateNode* node, int dimension);

bool is_state_plausible(StateNode* state, int dimension, int char_sum);
bool is_state_magic(StateNode* state, int dimension, int char_sum);
bool is_state_special(StateNode* state, int dimension, int char_sum);

#endif /* STATE_H */