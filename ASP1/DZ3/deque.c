#include "deque.h"
#include <stdlib.h>
#include <string.h>

Deque* deque_init() {
	Deque* deque = (Deque*)malloc(sizeof(Deque));

	if(!deque)
		return NULL;

	deque->start = deque->end = -1;
	deque->size = 0;
	for(int i = 0; i < DEQUE_SIZE; i++)
		deque->array[i] = -1;

	return deque;
}

void deque_push_front(Deque* deque, int element) {
	if(deque->start == -1)
		deque->start = deque->end = 0;
	else
		deque->start = (deque->start + DEQUE_SIZE - 1) % DEQUE_SIZE;

	deque->array[deque->start] = element;
	deque->size++;
}

void deque_push_back(Deque* deque, int element) {
	if(deque->start == -1)
		deque->start = deque->end = 0;
	else
		deque->end = (deque->end + 1) % DEQUE_SIZE;

	deque->array[deque->end] = element;
	deque->size++;
}

int deque_pop_front(Deque* deque) {
	int data = deque->array[deque->start];

	deque->array[deque->start] = -1;
	deque->size--;

	if(deque->start == deque->end)
		deque->start = deque->end = -1;
	else
		deque->start = (deque->start + 1) % DEQUE_SIZE;

	return data;
}

int deque_pop_back(Deque* deque) {
	int data = deque->array[deque->end];

	deque->array[deque->end] = -1;
	deque->size--;

	if(deque->start == deque->end)
		deque->start = deque->end = -1;
	else
		deque->end = (deque->end + DEQUE_SIZE - 1) % DEQUE_SIZE;

	return data;
}

int deque_peek_front(Deque* deque) {
	return deque->array[deque->start];
}

int deque_peek_back(Deque* deque) {
	return deque->array[deque->end];
}

bool deque_empty(Deque* deque) {
	return deque->start == -1;
}
