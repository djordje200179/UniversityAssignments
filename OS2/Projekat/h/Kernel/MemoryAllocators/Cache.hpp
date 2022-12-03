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

	void* allocate();
	void deallocate(void* ptr);
private:
    size_t typeSize;

	Slab* headSlab = nullptr;
};
}
}