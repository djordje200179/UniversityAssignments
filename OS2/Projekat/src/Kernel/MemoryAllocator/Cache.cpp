#include "../../../h/Kernel/MemoryAllocators/Cache.hpp"
#include "../../../h/syscall_cpp.hpp"
#include "../../../h/slab.h"

Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::cachesBlock;
unsigned int Kernel::MemoryAllocators::Cache::cachesCount;
Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::cachesHead;
Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::bufferCaches[MAX_BUFFER_CACHE_DEGREE - MIN_BUFFER_CACHE_DEGREE + 1];
Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::bigSlabsCache;

static const char* NOT_ENOUGH_MEMORY_ERROR = "ERROR: Not enough memory";
static const char* OBJECT_NOT_FROM_CACHE_ERROR = "ERROR: Object doesn't belong to this cache";


void Kernel::MemoryAllocators::Cache::initCachesBlock() {
	cachesBlock = (Cache*)Buddy::getInstance().allocate(1);
	cachesCount = 0;
	cachesHead = nullptr;
}

void Kernel::MemoryAllocators::Cache::initBufferCaches() {
	for (size_t i = 0; i < MAX_BUFFER_CACHE_DEGREE - MIN_BUFFER_CACHE_DEGREE + 1; i++)
		bufferCaches[i] = nullptr;
}

void Kernel::MemoryAllocators::Cache::initBigSlabsCache() {
	bigSlabsCache = new Cache(sizeof(Slab) + sizeof(uint16), "Big slabs", nullptr, nullptr);
}

Kernel::MemoryAllocators::Cache* Kernel::MemoryAllocators::Cache::getBufferCache(size_t degree) {
	auto index = degree - MIN_BUFFER_CACHE_DEGREE;
	
	if (!bufferCaches[index]) {
		char name[] = "Buffer - 2^xx";
		name[11] = '0' + degree / 10;
		name[12] = '0' + degree % 10;

		bufferCaches[index] = new Cache(1 << degree, name, nullptr, nullptr);
	}
	
	return bufferCaches[index];
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
		if (fullyFreeSlabsHead) {
			partialSlabsHead = fullyFreeSlabsHead;
			fullyFreeSlabsHead = fullyFreeSlabsHead->nextSlab;
			partialSlabsHead->nextSlab = nullptr;
		} else {
			partialSlabsHead = (
				Slab::canFitIntoBlock(typeSize) 
				? new Slab(typeSize, ctor, dtor) 
				: new (bigSlabsCache) Slab(typeSize, ctor, dtor)
			);

			if (!partialSlabsHead) {
				error = NOT_ENOUGH_MEMORY_ERROR;
				return nullptr;
			}
			
			canShrink = false;
		}
	}

	void* ret = partialSlabsHead->allocate();

	if (!ret) {
		error = NOT_ENOUGH_MEMORY_ERROR;
		return nullptr;
	}

	if (partialSlabsHead->hasNoFreeSlots()) {
		auto temp = partialSlabsHead;
		partialSlabsHead = partialSlabsHead->nextSlab;
		temp->nextSlab = fullyAllocatedSlabsHead;
		fullyAllocatedSlabsHead = temp;
	}

	return ret;
}

bool Kernel::MemoryAllocators::Cache::deallocate(void* ptr) {
	for (auto prevSlab = (Slab*)nullptr, currSlab = partialSlabsHead; currSlab; prevSlab = currSlab, currSlab = currSlab->nextSlab) {
		if (!currSlab->deallocate(ptr))
			continue;

		if (currSlab->hasNoAllocatedSlots()) {
			(prevSlab ? prevSlab->nextSlab : partialSlabsHead) = currSlab->nextSlab;
			currSlab->nextSlab = fullyFreeSlabsHead;
			fullyFreeSlabsHead = currSlab;
		}

		return true;
	}

	for (auto prevSlab = (Slab*)nullptr, currSlab = fullyAllocatedSlabsHead; currSlab; prevSlab = currSlab, currSlab = currSlab->nextSlab) {
		if (!currSlab->deallocate(ptr))
			continue;

		if (currSlab->hasNoAllocatedSlots()) {
			(prevSlab ? prevSlab->nextSlab : fullyAllocatedSlabsHead) = currSlab->nextSlab;
			currSlab->nextSlab = fullyFreeSlabsHead;
			fullyFreeSlabsHead = currSlab;
		} else {
			(prevSlab ? prevSlab->nextSlab : fullyAllocatedSlabsHead) = currSlab->nextSlab;
			currSlab->nextSlab = partialSlabsHead;
			partialSlabsHead = currSlab;
		}

		return true;
	}

	error = OBJECT_NOT_FROM_CACHE_ERROR;
	return false;
}

size_t Kernel::MemoryAllocators::Cache::shrink() {
	if (!canShrink) {
		canShrink = true;
		return 0;
	}

	size_t res = 0;
	for (auto currSlab = fullyFreeSlabsHead; currSlab;) {
		auto nextSlab = currSlab->nextSlab;

		res += 1;
		delete currSlab;

		currSlab = nextSlab;
	}
	fullyFreeSlabsHead = nullptr;

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
	} else if (fullyAllocatedSlabsHead) {
		slotsPerSlab = fullyAllocatedSlabsHead->getNumOfSlots();
		allocatedBlocks = fullyAllocatedSlabsHead->getAllocatedBlocks();
	} else if (fullyFreeSlabsHead) {
		slotsPerSlab = fullyFreeSlabsHead->getNumOfSlots();
		allocatedBlocks = fullyFreeSlabsHead->getAllocatedBlocks();
	}

	Console::puts("Slots per slab: ");
	Console::puti(slotsPerSlab);
	Console::putc('\n');

	Console::puts("Allocated blocks: ");
	Console::puti(allocatedBlocks);
	Console::putc('\n');

	Console::puts("----------------\n");
	
	size_t allocatedSlots = 0, numOfSlabs = 0;

	if (fullyAllocatedSlabsHead) {
		Console::puts("Full slabs: ");
		
		for (auto currSlab = fullyAllocatedSlabsHead; currSlab; currSlab = currSlab->nextSlab)
			numOfSlabs++;
		
		Console::puti(numOfSlabs);
		Console::putc('\n');
		
		allocatedSlots += slotsPerSlab * numOfSlabs;
	}

	if (partialSlabsHead) {
		Console::puts("Partially full slabs: \n");
		
		size_t i = 1;
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
	}

	if (fullyFreeSlabsHead) {
		Console::puts("Empty slabs: \n");
		
		auto initialNumOfSlabs = numOfSlabs;
		for (auto currSlab = fullyFreeSlabsHead; currSlab; currSlab = currSlab->nextSlab)
			numOfSlabs++;

		Console::puti(numOfSlabs - initialNumOfSlabs);
		Console::putc('\n');
	}

	Console::puts("----------------\n");

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
	if(!error)
		return 0;
	
	Console::puts(error);
	return 1;
}
