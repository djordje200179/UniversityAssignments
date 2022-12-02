#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"

namespace Kernel {
namespace MemoryAllocators {
template<typename T>
class Cache;

template<typename T>
class Slab {
// Misc
public:
	friend class Cache<T>;

	BUDDY_ALLOCATED(Slab<T>);

	static const int SLOTS = 10;
// Nonstatic members
public:
	Slab() {
        for (size_t i = 0; i < SLOTS - 1; i++)
            *(T**)(&slots[i]) = &slots[i + 1];
        *(T**)(&slots[SLOTS - 1]) = 0;
    }

	T* allocate() {
        if (!freeSlot)
            return nullptr;

        T* ret = freeSlot;
        freeSlot = *(T**)freeSlot;
        return ret;
    }

	bool deallocate(T* ptr) {
        if (slots > ptr || ptr >= slots + SLOTS)
            return false;

        *(T**)ptr = freeSlot;
        freeSlot = ptr;

        return true;
    }
private:
    char buffer[SLOTS * sizeof(T)] = {0};

    T* slots = (T*)buffer;
    T* freeSlot = &slots[0];
	Slab<T>* next = nullptr;
};
}
}