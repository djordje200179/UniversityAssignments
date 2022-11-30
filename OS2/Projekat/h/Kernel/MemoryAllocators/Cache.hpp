#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

namespace Kernel {
namespace MemoryAllocators {
template<typename T>
class Cache<T> {
public:
	Cache(const Cache&) = delete;
	Cache& operator=(const Cache&) = delete;

	static Cache<T>& getInstance() {
		static Cache<T>* instance = new Cache<T>();
		return *instance;
	}
private:
	BUDDY_ALLOCATED(Cache<T>);

	Cache() = default;

public:
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

#define CACHE_ALLOCATED(type)                                                                   \
	static void* operator new(size_t size) { return Cache<type>::getInstance().allocate(); }    \
	static void operator delete(void* ptr) { Cache<type>::getInstance().deallocate(ptr); }      \