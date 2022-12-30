#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

namespace Kernel {
namespace MemoryAllocators {
class Cache {
	// Misc
public:
	//BUDDY_ALLOCATED(Cache);
private:
	static size_t calculateSlotsPerSlab(size_t blockSize, size_t typeSize);
	// Nonstatic members
public:
	Cache(size_t typeSize) : typeSize(typeSize) {
		slotsPerSlab = calculateSlotsPerSlab(4096, typeSize);
	}

	void* allocate();

	void deallocate(void* ptr);

private:
	void allocateNewSlab();
	
	size_t typeSize;
	size_t slotsPerSlab;

	Slab* fullSlabHead = nullptr;
	Slab* partialSlabHead = nullptr;
	Slab* emptySlabHead = nullptr;
};
}
}

#define CACHE_ALLOCATED(T)                                                  \
	public:                                                                 \
	    void* operator new(size_t size) { return getCache().allocate(); }   \
	    void operator delete(void* ptr) { getCache().deallocate(ptr); }     \
	private:                                                                \
	    static MemoryAllocators::Cache& getCache() {                        \
	        static MemoryAllocators::Cache* instance = new MemoryAllocators::Cache(sizeof(T)); \
	        return *instance;                                               \
	    }
