#pragma once

#include "../../../lib/hw.h"

namespace Kernel {
namespace MemoryAllocators {
class Heap {
public:
	Heap(const Heap&) = delete;
	Heap& operator=(const Heap&) = delete;

	static Heap& getInstance() {
		static Heap instance;
		return instance;
	}
private:
	Heap();

public:
	void* allocate(size_t blocks);
	void deallocate(void* ptr);

	static size_t calculateBlocks(size_t bytes) {
		bytes += sizeof(size_t);

		size_t blocks = bytes / MEM_BLOCK_SIZE;
		if (bytes % MEM_BLOCK_SIZE)
			blocks++;

		return blocks;
	}

private:
	struct MemorySegment {
		size_t blocks;

		MemorySegment* prev;
		MemorySegment* next;

		void tryJoinWithNext();
	};

	MemorySegment* headSegment;
};
}
}