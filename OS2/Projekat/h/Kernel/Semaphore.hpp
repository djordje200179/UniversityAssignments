#pragma once

#include "MemoryAllocators/Cache.hpp"

namespace Kernel {
class Thread;

class Semaphore {
// Misc
public:
	CACHE_ALLOCATED(Semaphore);

// Nonstatic members
public:
	KERNEL_ALLOCATOR

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