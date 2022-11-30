#include "../../../h/Kernel/MemoryAllocators/Heap.hpp"

Kernel::MemoryAllocators::Heap::Heap() {
	headSegment = (MemorySegment*)HEAP_START_ADDR;

	headSegment->blocks = (uint64)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR) / MEM_BLOCK_SIZE;
	headSegment->prev = nullptr;
	headSegment->next = nullptr;
}

void* Kernel::MemoryAllocators::Heap::allocate(size_t blocks) {
	MemorySegment* validSegment;
	for(validSegment = headSegment; validSegment; validSegment = validSegment->next)
		if(validSegment->blocks >= blocks)
			break;

	if(!validSegment)
		return nullptr;

	auto prevSegment = validSegment->prev;
	auto nextSegment = validSegment->next;

	if(validSegment->blocks > blocks) {
		auto newSegment = (MemorySegment*)((char*)validSegment + MEM_BLOCK_SIZE * blocks);

		newSegment->blocks = validSegment->blocks - blocks;

		(prevSegment ? prevSegment->next : headSegment) = newSegment;
		newSegment->prev = prevSegment;

		if(nextSegment)
			nextSegment->prev = newSegment;
		newSegment->next = nextSegment;
	} else {
		(prevSegment ? prevSegment->next : headSegment) = nextSegment;
		if(nextSegment)
			nextSegment->prev = prevSegment;
	}

	auto headerPointer = (size_t*)validSegment;
	*headerPointer = blocks;
	headerPointer++;

	return headerPointer;
}

void Kernel::MemoryAllocators::Heap::deallocate(void* ptr) {
	auto headerPointer = (size_t*)ptr - 1;
	auto blocks = *headerPointer;
	ptr = headerPointer;

	MemorySegment* prevSegment;
	for(prevSegment = headSegment; prevSegment; prevSegment = prevSegment->next)
		if(ptr < prevSegment)
			break;
	prevSegment = prevSegment->prev;

	auto newSegment = (MemorySegment*)ptr;
	newSegment->blocks = blocks;
	newSegment->prev = prevSegment;

	newSegment->next = prevSegment ? prevSegment->next : headSegment;
	(prevSegment ? prevSegment->next : headSegment) = newSegment;

	if(newSegment->next)
		newSegment->next->prev = newSegment;

	newSegment->tryJoinWithNext();
	if(prevSegment)
		prevSegment->tryJoinWithNext();
}

void Kernel::MemoryAllocators::Heap::MemorySegment::tryJoinWithNext() {
	if(!next || (char*)this + blocks * MEM_BLOCK_SIZE != (char*)next)
		return;

	blocks += next->blocks;
	next = next->next;
	if(next)
		next->prev = this;
}
