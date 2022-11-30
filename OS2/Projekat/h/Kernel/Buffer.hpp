#pragma once

#include "MemoryAllocators/Heap.hpp"

namespace Kernel {
class Buffer {
public:
	KERNEL_ALLOCATOR

	static const int BUFFER_SIZE = 256;

	void put(char c);
	char get();

	bool empty() const { return count == 0; }
	bool full() const { return count == BUFFER_SIZE; }
private:
	char buffer[BUFFER_SIZE];
	int head = 0, tail = 0, count = 0;
};
}