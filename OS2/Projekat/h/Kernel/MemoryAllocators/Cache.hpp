#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

namespace Kernel {
namespace MemoryAllocators {
class Cache {
// Misc
public:
	BUDDY_ALLOCATED(Cache);
public:
	Cache(size_t typeSize) : typeSize(typeSize) {}

	void *allocate();

	void deallocate(void *ptr);

private:
	size_t typeSize;

	Slab *headSlab = nullptr;
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
