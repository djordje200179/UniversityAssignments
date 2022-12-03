#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"

namespace Kernel {
namespace MemoryAllocators {
class Slab {
// Misc
public:
	friend class Cache;

    static void* operator new(size_t size, size_t typeSize, size_t numOfSlots);
	static void operator delete(void* ptr);

// Nonstatic members
public:
	Slab(size_t typeSize, size_t numOfSlots);

	void* allocate();
	bool deallocate(void* ptr);
private:
    void* getSlotsStart() { return this + 1; }
    void* getSlotsEnd() { return (char*)(this + 1) + numOfSlots * typeSize; }

    void* getSlot(size_t index) { return (char*)getSlotsStart() + index * typeSize; }

    size_t typeSize;
    size_t numOfSlots;

    void* freeSlot = this + 1;
	Slab* next = nullptr;
};
}
}