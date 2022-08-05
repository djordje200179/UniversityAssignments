#pragma once

#include "../../lib/hw.h"
#include "Singleton.hpp"

namespace Kernel {
SINGLETON_CLASS(MemoryAllocator)
public:
	void* allocate(size_t blocks);
	void* allocateBytes(size_t bytes) {	return allocate(calculateBlocks(bytes));	}

	void deallocate(void* ptr);

	static size_t calculateBlocks(size_t bytes) {
		bytes += sizeof(size_t);

		size_t blocks = bytes / MEM_BLOCK_SIZE;
		if(bytes % MEM_BLOCK_SIZE)
			blocks++;

		return blocks;
	}
private:
	MemoryAllocator();

	struct MemorySegment {
		size_t blocks;

		MemorySegment* prev;
		MemorySegment* next;

		void tryJoinWithNext();
	};

	MemorySegment* headSegment;
};
}

#define KERNEL_ALLOCATOR 																							\
	static void* operator new(size_t size) { return Kernel::MemoryAllocator::getInstance().allocateBytes(size); }	\
	static void operator delete(void* ptr) { Kernel::MemoryAllocator::getInstance().deallocate(ptr); }
