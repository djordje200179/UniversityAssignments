#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

namespace Kernel {
namespace MemoryAllocators {
template<typename T>
class Cache<T> {
// Misc
public:
	BUDDY_ALLOCATED(Cache<T>);

	friend class Slab;
// Nonstatic members
public:
	Cache();

	T* allocate();
	void deallocate(T* ptr);
private:
	Slab* headSlab = nullptr;
};
}
}

template<typename T>
T* Kernel::MemoryAllocators::Cache<T>::allocate() {
	for (Slab* slab = headSlab; slab; slab = slab->nextSlab) {
		T* ret = slab->allocate();
		if (ret)
			return ret;
	}

	Slab* newSlab = new Slab();

	if (!newSlab)
		return nullptr;

	newSlab->nextSlab = headSlab;
	headSlab = newSlab;

	return headSlab->allocate();
}

template<typename T>
void Kernel::MemoryAllocators::Cache<T>::deallocate(T* ptr) {
	for (Slab* slab = headSlab; slab; slab = slab->nextSlab) {
		bool success = slab->deallocate(ptr);

		if (success)
			return;
	}
}