#ifndef DEQUE_H
#define DEQUE_H

#include <stdbool.h>

#define DEQUE_SIZE 100

typedef struct Deque {
	int start;
	int end;
	int size;

	int array[DEQUE_SIZE];
} Deque;

Deque* deque_init();

void deque_push_front(Deque* deque, int element);
void deque_push_back(Deque* deque, int element);

int deque_pop_front(Deque* deque);
int deque_pop_back(Deque* deque);

int deque_peek_front(Deque* deque);
int deque_peek_back(Deque* deque);

bool deque_empty(Deque* deque);

#endif /* DEQUE_H */