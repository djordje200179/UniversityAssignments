#include "../../../h/Kernel/MemoryAllocators/Cache.hpp"

size_t Kernel::MemoryAllocators::Cache::calculateSlotsPerSlab(size_t blockSize, size_t typeSize) {
	return size_t();
}

void* Kernel::MemoryAllocators::Cache::allocate() {
	for (auto prevSlab = (Slab*)nullptr, currSlab = partialSlabHead; currSlab; prevSlab = currSlab, currSlab = currSlab->next) {
		void* ret = currSlab->allocate();

		if (!ret)
			continue;

		if (currSlab->isEmpty()) {
			(prevSlab ? prevSlab->next : partialSlabHead) = currSlab->next;
			currSlab->next = emptySlabHead;
			emptySlabHead = currSlab;
		}

		return ret;
	}

	auto newSlab = new (typeSize, slotsPerSlab) Slab(typeSize, slotsPerSlab);
	if (!newSlab)
		return nullptr;

	newSlab->next = partialSlabHead;
	partialSlabHead = newSlab;

	return fullSlabHead->allocate();
}

void Kernel::MemoryAllocators::Cache::deallocate(void* ptr) {
	for (auto prevSlab = (Slab*)nullptr, currSlab = emptySlabHead; currSlab; prevSlab = currSlab, currSlab = currSlab->next) {
		bool success = currSlab->deallocate(ptr);

		if (!success)
			continue;

		if (currSlab->isFull()) {
			(prevSlab ? prevSlab->next : emptySlabHead) = currSlab->next;
			currSlab->next = partialSlabHead;
			partialSlabHead = currSlab;
		}
	}
}