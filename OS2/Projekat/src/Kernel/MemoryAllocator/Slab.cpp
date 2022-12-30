#include "../../../h/Kernel/MemoryAllocators/Slab.hpp"
#include "../../../h/slab.h"

Kernel::MemoryAllocators::Slab::Slab(size_t typeSize, OBJ_FUN ctor, OBJ_FUN dtor) :
    typeSize(typeSize),
    numOfSlots((BLOCK_SIZE - sizeof(Slab)) / (typeSize + sizeof(uint16))),
	slots(getFreeSlotsList() + numOfSlots),
	dtor(dtor) {    
	if (ctor)
		for (size_t i = 0; i < numOfSlots; i++)
			ctor(getSlot(i));
}

Kernel::MemoryAllocators::Slab::~Slab() {
	if (dtor)
		for (size_t i = 0; i < numOfSlots; i++)
			dtor(getSlot(i));
}

void* Kernel::MemoryAllocators::Slab::allocate() {
    if (freeSlotIndex == -1)
        return nullptr;

	void* ret = getSlot(freeSlotIndex);
	freeSlotIndex = getFreeSlotsList()[freeSlotIndex];
	allocatedSlots++;
    
    return ret;
}

bool Kernel::MemoryAllocators::Slab::deallocate(void* ptr) {
    if (getSlot(0) < (char*)ptr || (char*)ptr > getSlot(numOfSlots - 1))
        return false;

	size_t index = ((char*)ptr - (char*)slots) / typeSize;
	getFreeSlotsList()[index] = freeSlotIndex;
	freeSlotIndex = index;
	allocatedSlots--;

    return true;
}