#include "../../../h/Kernel/MemoryAllocators/Slab.hpp"

void* Kernel::MemoryAllocators::Slab::operator new(size_t size, size_t typeSize, size_t numOfSlots) {
    auto slotsSize = numOfSlots * typeSize;
    auto totalSize = size + slotsSize;

    return Buddy::getInstance().allocate(totalSize);
}

void Kernel::MemoryAllocators::Slab::operator delete(void* ptr) {
    auto slab = reinterpret_cast<Slab*>(ptr);

    auto slotsSize = slab->numOfSlots * slab->typeSize;
    auto totalSize = sizeof(Slab) + slotsSize;

    Buddy::getInstance().deallocate(ptr, totalSize);
}

Kernel::MemoryAllocators::Slab::Slab(size_t typeSize, size_t numOfSlots)
    : typeSize(typeSize), numOfSlots(numOfSlots) {
    for (size_t i = 0; i < numOfSlots - 1; i++)
        *(void**)getSlot(i) = getSlot(i + 1);
    *(void**) getSlot(numOfSlots - 1) = nullptr;
}

void* Kernel::MemoryAllocators::Slab::allocate() {
    if (!freeSlot)
        return nullptr;

    void* ret = freeSlot;
    freeSlot = *(void**)freeSlot;
    return ret;
}

bool Kernel::MemoryAllocators::Slab::deallocate(void* ptr) {
    if (getSlotsStart() > (char*)ptr || (char*)ptr >= getSlotsEnd())
        return false;

    *(void**)ptr = freeSlot;
    freeSlot = ptr;

    return true;
}