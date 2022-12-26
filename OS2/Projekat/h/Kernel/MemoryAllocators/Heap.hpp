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
	struct Segment {
		static Segment* getSegment(void* data) { return (Segment*)((size_t*)data - 1); }
		
		size_t blocks;

		union {
			struct {
				Segment* prev;
				Segment* next;
			};

			char data[0];
		};
	};

	void joinBlocks(Segment* middle);

	Segment* headSegment;
};
}
}