#include "../../../h/Kernel/MemoryAllocators/Cache.hpp"

void* Kernel::MemoryAllocators::Cache::allocate() {
    for (auto slab = headSlab; slab; slab = slab->next) {
        void* ret = slab->allocate();
        if (ret)
            return ret;
    }

    auto newSlab = new (typeSize, 10) Slab(typeSize, 10);
    if (!newSlab)
        return nullptr;

    newSlab->next = headSlab;
    headSlab = newSlab;

    return headSlab->allocate();
}

void Kernel::MemoryAllocators::Cache::deallocate(T* ptr) {
    for (auto slab = headSlab; slab; slab = slab->next) {
        bool success = slab->deallocate(ptr);

        if (success)
            return;
    }
}