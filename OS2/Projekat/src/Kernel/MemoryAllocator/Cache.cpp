#include "../../../h/Kernel/MemoryAllocators/Cache.hpp"
#include "../../../h/syscall_cpp.hpp"

void* Kernel::MemoryAllocators::Cache::cachesBlock;
unsigned int Kernel::MemoryAllocators::Cache::cachesCount;
Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::cachesHead;

void Kernel::MemoryAllocators::Cache::initCachesBlock() {
	cachesBlock = Buddy::getInstance().allocate(1);
	cachesCount = 0;
	cachesHead = nullptr;
}

void* Kernel::MemoryAllocators::Cache::operator new(size_t size) {
	Cache* freeSlot = (Cache*)((size_t*)cachesBlock + cachesCount);
	cachesCount++;
	
	freeSlot->nextCache = cachesHead;
	cachesHead = freeSlot;
	
	return freeSlot;
}

void Kernel::MemoryAllocators::Cache::operator delete(void* ptr) {
	for (auto prevCache = (Cache*)nullptr, currCache = cachesHead; currCache; prevCache = currCache, currCache = currCache->nextCache) {
		if (currCache != (Cache*)ptr)
			continue;

		(prevCache ? prevCache->nextCache : cachesHead) = currCache->nextCache;
	}
}

void* Kernel::MemoryAllocators::Cache::allocate() {
	if (!partialSlabsHead) {
		if (fullSlabsHead) {
			partialSlabsHead = fullSlabsHead;
			fullSlabsHead = fullSlabsHead->nextSlab;
			partialSlabsHead->nextSlab = nullptr;
		} else {
			partialSlabsHead = new Slab(typeSize, ctor, dtor);
			canShrink = false;
		}		
	}
	
	void* ret = partialSlabsHead->allocate();

	if (partialSlabsHead->isEmpty()) {
		auto temp = partialSlabsHead;
		partialSlabsHead = partialSlabsHead->nextSlab;
		temp->nextSlab = fullSlabsHead;
		fullSlabsHead = temp;
	}

	return ret;
}

bool Kernel::MemoryAllocators::Cache::deallocate(void* ptr) {
	for (auto prevSlab = (Slab*)nullptr, currSlab = partialSlabsHead; currSlab; prevSlab = currSlab, currSlab = currSlab->nextSlab) {
		if (!currSlab->deallocate(ptr))
			continue;
		
		if (currSlab->isFull()) {
			(prevSlab ? prevSlab->nextSlab : partialSlabsHead) = currSlab->nextSlab;
			currSlab->nextSlab = emptySlabsHead;
			emptySlabsHead = currSlab;
		}

		return true;
	}

	for (auto prevSlab = (Slab*)nullptr, currSlab = emptySlabsHead; currSlab; prevSlab = currSlab, currSlab = currSlab->nextSlab) {
		if (!currSlab->deallocate(ptr))
			continue;
		
		(prevSlab ? prevSlab->nextSlab : emptySlabsHead) = currSlab->nextSlab;
		currSlab->nextSlab = partialSlabsHead;
		partialSlabsHead = currSlab;

		return true;
	}

	return false;
}

size_t Kernel::MemoryAllocators::Cache::shrink() {
	if (!canShrink) {
		canShrink = true;
		return 0;
	}

	size_t res = 0;
	for (auto currSlab = emptySlabsHead; currSlab;) {
		auto nextSlab = currSlab->nextSlab;

		res += 1;
		delete currSlab;

		currSlab = nextSlab;
	}
	emptySlabsHead = nullptr;
	
	return res;
}

void Kernel::MemoryAllocators::Cache::printInfo() {
	Console::puts(name);
}

int Kernel::MemoryAllocators::Cache::printError() {
	return 0;
}
