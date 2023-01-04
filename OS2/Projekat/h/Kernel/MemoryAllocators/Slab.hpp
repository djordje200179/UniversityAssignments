#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"

namespace Kernel {
namespace MemoryAllocators {
class Cache;

class Slab {
// Misc
public:
	friend class Cache;
	
	using OBJ_FUN = void (*)(void*);

// Static members
public:
	static void* operator new(size_t size) { return Buddy::getInstance().allocate(1); }
	static void operator delete(void* ptr) { Buddy::getInstance().deallocate(ptr, 1); }

	static void* operator new(size_t size, Cache* cache);
	static void operator delete(void* ptr, Cache* cache);
	
	static bool canFitIntoBlock(size_t typeSize);

// Nonstatic members
public:
	Slab(size_t typeSize, OBJ_FUN ctor, OBJ_FUN dtor);
	~Slab();

	void* allocate();
	bool deallocate(void* ptr);

	size_t getAllocatedSlots() { return allocatedSlots; }
	bool hasNoAllocatedSlots() { return allocatedSlots == 0; }
	bool hasNoFreeSlots() { return allocatedSlots == numOfSlots; }

	size_t getAllocatedBlocks() { return 1; }
	size_t getNumOfSlots() { return numOfSlots; }
	
private:
	uint16* getFreeSlotsList() { return (uint16*)(this + 1); }
	void* getSlot(size_t index) { return (char*)slots + index * typeSize; }

    size_t typeSize, numOfSlots;
	OBJ_FUN dtor;
	
	Slab* nextSlab = nullptr;
	
	void* slots;
	uint16 freeSlotIndex = 0;
	size_t allocatedSlots = 0;
};
}
}