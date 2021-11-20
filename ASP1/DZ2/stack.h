#ifndef STACK_H
#define STACK_H

#include "state.h"
#include <stdbool.h>

typedef struct StackNode {
	struct StateNode* info;

	struct StackNode* next;
	struct StackNode* prev;
} StackNode;

typedef struct Stack {
	StackNode* head;
	StackNode* tail;
} Stack;

StackNode* init_stack_node(StateNode* data);
Stack* init_stack();

StateNode* stack_pop(Stack* stack);
void stack_push(Stack* stack, StateNode* data);
bool stack_empty(Stack* stack);

#endif /* STACK_H */