#include "../../../h/Kernel/MemoryAllocators/Slab.hpp"
#include "../../../h/Kernel/MemoryAllocators/Cache.hpp"
#include "../../../h/slab.h"

void* Kernel::MemoryAllocators::Slab::operator new(size_t size, Cache* cache) { 
	return cache->allocate();
}

void Kernel::MemoryAllocators::Slab:: operator delete(void* ptr, Cache* cache) { 
	cache->deallocate(ptr);
}

bool Kernel::MemoryAllocators::Slab::canFitIntoBlock(size_t typeSize) {
	auto totalSize = sizeof(Slab) + typeSize + sizeof(uint16);
	
	return totalSize <= BLOCK_SIZE;
}

Kernel::MemoryAllocators::Slab::Slab(size_t typeSize, OBJ_FUN ctor, OBJ_FUN dtor) :
	typeSize(typeSize), numOfSlots(),
	dtor(dtor) {
	if (canFitIntoBlock(typeSize)) {
		numOfSlots = (BLOCK_SIZE - sizeof(Slab)) / (typeSize + sizeof(uint16));
		slots = getFreeSlotsList() + numOfSlots;
	} else {
		numOfSlots = 1;
		slots = Buddy::getInstance().allocate((typeSize + BLOCK_SIZE - 1) / BLOCK_SIZE);
		// TODO: what if no memory?
	}
	
	auto slotsList = getFreeSlotsList();
	for (uint16 i = 0; i < numOfSlots + 1; i++) 
		slotsList[i] = i + 1;
	slotsList[numOfSlots - 1] = (uint16)-1;
	
	if (ctor)
		for (size_t i = 0; i < numOfSlots; i++)
			ctor(getSlot(i));
}

Kernel::MemoryAllocators::Slab::~Slab() {
	if (dtor)
		for (size_t i = 0; i < numOfSlots; i++)
			dtor(getSlot(i));

	if (!canFitIntoBlock(typeSize))
		Buddy::getInstance().deallocate(slots, (typeSize + BLOCK_SIZE - 1) / BLOCK_SIZE);
}

void* Kernel::MemoryAllocators::Slab::allocate() {
	if (freeSlotIndex == (uint16)-1)
		return nullptr;

	void* ret = getSlot(freeSlotIndex);
	freeSlotIndex = getFreeSlotsList()[freeSlotIndex];
	allocatedSlots++;

	return ret;
}

bool Kernel::MemoryAllocators::Slab::deallocate(void* ptr) {
	if ((char*)ptr < getSlot(0) || (char*)ptr > getSlot(numOfSlots - 1))
		return false;

	size_t index = ((char*)ptr - (char*)slots) / typeSize;
	getFreeSlotsList()[index] = freeSlotIndex;
	freeSlotIndex = index;
	allocatedSlots--;

	return true;
}