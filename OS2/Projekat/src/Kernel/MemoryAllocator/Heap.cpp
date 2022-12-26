#include "../../../h/Kernel/MemoryAllocators/Heap.hpp"

Kernel::MemoryAllocators::Heap::Heap() {
	auto startAddress = (char*)((uint64)HEAP_START_ADDR + ((uint64)HEAP_START_ADDR >> 3));
	auto endAddress = (char*)(HEAP_END_ADDR);

	headSegment = (Segment*)startAddress;
	headSegment->blocks = (uint64)(endAddress - startAddress) / MEM_BLOCK_SIZE;
	headSegment->prev = nullptr;
	headSegment->next = nullptr;
}

void* Kernel::MemoryAllocators::Heap::allocate(size_t blocks) {
	Segment* validSegment;
	for (validSegment = headSegment; validSegment; validSegment = validSegment->next)
		if (validSegment->blocks >= blocks)
			break;

	if (!validSegment)
		return nullptr;

	auto prevSegment = validSegment->prev;
	auto nextSegment = validSegment->next;

	if (validSegment->blocks > blocks) {
		auto newSegment = (Segment*)((char*)validSegment + MEM_BLOCK_SIZE * blocks);

		newSegment->blocks = validSegment->blocks - blocks;

		(prevSegment ? prevSegment->next : headSegment) = newSegment;
		newSegment->prev = prevSegment;

		if (nextSegment)
			nextSegment->prev = newSegment;
		newSegment->next = nextSegment;
	} else {
		(prevSegment ? prevSegment->next : headSegment) = nextSegment;
		if (nextSegment)
			nextSegment->prev = prevSegment;
	}

	validSegment->blocks = blocks;
	return validSegment->data;
}

void Kernel::MemoryAllocators::Heap::deallocate(void* ptr) {
	auto segment = Segment::getSegment(ptr);

	Segment* prevSegment;
	Segment* nextSegment;
	for(prevSegment = nullptr, nextSegment = headSegment; nextSegment; prevSegment = nextSegment, nextSegment = nextSegment->next)
		if(nextSegment > segment)
			break;
	
	segment->prev = prevSegment;
	segment->next = prevSegment ? prevSegment->next : headSegment;
	(prevSegment ? prevSegment->next : headSegment) = segment;
	if (segment->next)
		segment->next->prev = segment;

	joinBlocks(segment);
}

void Kernel::MemoryAllocators::Heap::joinBlocks(Segment* middle) {
	if(middle->next && (char*)middle + middle->blocks * MEM_BLOCK_SIZE == (char*)middle->next) {
		middle->blocks += middle->next->blocks;
		middle->next = middle->next->next;
	}

	if(middle->prev && (char*)middle->prev + middle->prev->blocks * MEM_BLOCK_SIZE == (char*)middle) {
		middle->prev->blocks += middle->blocks;
		middle->prev->next = middle->next;
	}
}