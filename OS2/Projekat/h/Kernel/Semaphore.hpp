#pragma once

#include "MemoryAllocators/Cache.hpp"

namespace Kernel {
class Thread;

class Semaphore {
// Misc
public:
    static void* operator new(size_t size) {
        if(!cache)
            cache = new MemoryAllocators::Cache(sizeof(Semaphore));

        return cache->allocate();
    }

    static void operator delete(void* ptr) { cache->deallocate(ptr); }
private:
    static MemoryAllocators::Cache* cache;

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