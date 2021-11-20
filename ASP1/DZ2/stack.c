#include "stack.h"
#include "state.h"
#include <stdlib.h>
#include <stdbool.h>

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

bool stack_empty(Stack* stack) {
	return stack->head == NULL;
}