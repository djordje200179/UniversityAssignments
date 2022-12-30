#pragma once

#include "../cache_allocated.h"

namespace Kernel {
class Thread;

class Semaphore {
// Misc
public:
	CACHE_ALLOCATED(Semaphore);

// Nonstatic members
public:
	Semaphore(unsigned initialValue) { value = initialValue; }
	~Semaphore();

	void wait();
	void signal();
private:
	void block();
	void unblock();

	int value;

	Thread* head = nullptr;
	Thread* tail = nullptr;
};
}