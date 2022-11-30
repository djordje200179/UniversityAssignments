#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"

namespace Kernel {
namespace MemoryAllocators {
template<typename T>
class Slab<T> {
// Misc
	BUDDY_ALLOCATED(Slab<T>);

	const int SLOTS = 10;
// Nonstatic members
public:
	Slab();

	T* allocate();
	bool deallocate(T* ptr);
private:
	Slab<T> next = nullptr;
	T* freeSlot = &slots[0];
	T slots[SLOTS]
};
}
}

template<typename T>
Kernel::MemoryAllocators::Slab<T>::Slab() {
	for (size_t i = 0; i < SLOTS - 1; i++)
		*(T**)(&slots[i]) = &slots[i + 1];
	*(T**)(&slots[SLOTS - 1]) = 0;
}

template<typename T>
T* Kernel::MemoryAllocators::Slab<T>::allocate() {
	if (!freeSlot)
		return nullptr;

	T* ret = freeSlot;
	freeSlot = *(T**)freeSlot;
	return ret;
}

template<typename T>
bool Kernel::MemoryAllocators::Slab<T>::deallocate(T* ptr) {
	if (slots > ptr || ptr >= slots + SLOTS)
		return false;

	*(T**)ptr = freeSlot;
	freeSlot = ptr;

	return true;
}