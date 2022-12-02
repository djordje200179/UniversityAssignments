#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

namespace Kernel {
namespace MemoryAllocators {
template<typename T>
class Cache {
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
	T* allocate() {
        for (auto slab = headSlab; slab; slab = slab->next) {
            T* ret = slab->allocate();
            if (ret)
                return ret;
        }

        auto newSlab = new Slab<T>();

        if (!newSlab)
            return nullptr;

        newSlab->next = headSlab;
        headSlab = newSlab;

        return headSlab->allocate();
    }

	void deallocate(T* ptr) {
        for (auto slab = headSlab; slab; slab = slab->next) {
            bool success = slab->deallocate(ptr);

            if (success)
                return;
        }
    }
private:
	Slab<T>* headSlab = nullptr;
};
}
}

#define CACHE_ALLOCATED(T)                                                                                              \
	static void* operator new(size_t size) { return Kernel::MemoryAllocators::Cache<T>::getInstance().allocate(); }     \
	static void operator delete(void* ptr) { Kernel::MemoryAllocators::Cache<T>::getInstance().deallocate((T*)ptr); }
