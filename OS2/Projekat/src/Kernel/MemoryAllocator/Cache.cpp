#include "../../../h/Kernel/MemoryAllocators/Cache.hpp"
#include "../../../h/syscall_cpp.hpp"
#include "../../../h/slab.h"

Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::cachesBlock;
unsigned int Kernel::MemoryAllocators::Cache::cachesCount;
Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::cachesHead;

void Kernel::MemoryAllocators::Cache::initCachesBlock() {
	cachesBlock = (Cache*)Buddy::getInstance().allocate(1);
	cachesCount = 0;
	cachesHead = nullptr;
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
	Console::puts("==CACHE: ");
	Console::puts(name);
	Console::puts("==\n");

	Console::puts("Type size: ");
	Console::puti(typeSize);
	Console::putc('B');
	Console::putc('\n');

	size_t slotsPerSlab = 0, allocatedBlocks = 0;
	if (partialSlabsHead) {
		slotsPerSlab = partialSlabsHead->getNumOfSlots();
		allocatedBlocks = partialSlabsHead->getAllocatedBlocks();
	} else if (fullSlabsHead) {
		slotsPerSlab = fullSlabsHead->getNumOfSlots();
		allocatedBlocks = fullSlabsHead->getAllocatedBlocks();
	} else if (emptySlabsHead) {
		slotsPerSlab = emptySlabsHead->getNumOfSlots();
		allocatedBlocks = emptySlabsHead->getAllocatedBlocks();
	}

	Console::puts("Slots per slab: ");
	Console::puti(slotsPerSlab);
	Console::putc('\n');

	Console::puts("Allocated blocks: ");
	Console::puti(allocatedBlocks);
	Console::putc('\n');

	Console::puts("----------------\n");
	
	size_t allocatedSlots = 0, numOfSlabs = 0, i = 0;

	if (fullSlabsHead) {
		Console::puts("Full slabs: \n");
		
		for (auto currSlab = fullSlabsHead; currSlab; currSlab = currSlab->nextSlab, i++) {
			Console::putc('\t');
			Console::puts(") ");
			Console::puti(i);

			Console::putc('\n');
			
			numOfSlabs++;
		}
		
		allocatedSlots += slotsPerSlab * numOfSlabs;
		
		Console::puts("----------------\n");
	}


	if (partialSlabsHead) {
		Console::puts("Partially full slabs: \n");
		
		for (auto currSlab = partialSlabsHead; currSlab; currSlab = currSlab->nextSlab, i++) {
			Console::putc('\t');
			Console::puti(i);
			Console::puts(") ");

			Console::puts("slots: ");
			Console::puti(currSlab->getAllocatedSlots());
			Console::putc('/');
			Console::puti(currSlab->getNumOfSlots());

			Console::putc('\n');

			allocatedSlots += currSlab->getAllocatedSlots();
			numOfSlabs++;
		}
		
		Console::puts("----------------\n");
	}

	if (emptySlabsHead) {
		Console::puts("Empty slabs: \n");
		
		for (auto currSlab = emptySlabsHead; currSlab; currSlab = currSlab->nextSlab, i++) {
			Console::putc('\t');
			Console::puti(i);
			Console::puts(") ");

			Console::putc('\n');
			
			numOfSlabs++;
		}
		
		Console::puts("----------------\n");
	}

	size_t totalSlots = slotsPerSlab * numOfSlabs;

	Console::puts("Allocation ratio: ");
	Console::puti(allocatedSlots);
	Console::putc('/');
	Console::puti(totalSlots);
	Console::puts(" (");
	Console::puti(allocatedSlots * 100 / totalSlots);
	Console::puts("%)");
	Console::putc('\n');

	Console::puts("====END INFO====\n");
	Console::putc('\n');
}

int Kernel::MemoryAllocators::Cache::printError() {
	return 0;
}
