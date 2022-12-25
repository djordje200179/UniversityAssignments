#include "../../../h/Kernel/MemoryAllocators/Buddy.hpp"

static uint64 clog2(uint64 num) {
    unsigned int result = 0;
    
    for(num--; num > 0; num >>= 1)
		result++;

    return result;
}


Kernel::MemoryAllocators::Buddy::Buddy() {
	for(size_t i = 0; i < BLOCK_SIZE_DEGRESS; i++)
		blocks[i] = nullptr;
	
	auto endAddress = (void*)((uint64)HEAP_END_ADDR - ((uint64)HEAP_END_ADDR >> 3));
	auto size = (uint64)endAddress - (uint64)HEAP_START_ADDR;
	auto degree = clog2(size);

	auto block = (FreeBlock*)HEAP_START_ADDR;
	block->prev = nullptr;
	block->next = nullptr;
	blocks[degree] = block;
}

void* Kernel::MemoryAllocators::Buddy::allocate(size_t size) {
	auto degree = clog2(size);
	
	auto freeDegree = findFreeDegree(degree);
	if(freeDegree == BLOCK_SIZE_DEGRESS)
		return nullptr;
	if(freeDegree > degree)
		recursiveSplit(degree, freeDegree);
	
	auto block = blocks[degree];
	removeBlock(block, degree);
	
	return block;
}

void Kernel::MemoryAllocators::Buddy::deallocate(void* ptr, size_t size) {
	if(ptr == nullptr)
		return;
	
	auto degree = clog2(size);

	auto block = (FreeBlock*)ptr;
	insertBlock(block, degree);

	recursiveJoin(block, degree);
}

void Kernel::MemoryAllocators::Buddy::insertBlock(FreeBlock* block, size_t degree) {
	auto prev = (FreeBlock*)nullptr, curr = blocks[degree];
	while(curr && curr < block)
		prev = curr, curr = curr->next;

	(prev ? prev->next : blocks[degree]) = block;
	block->prev = prev;
	block->next = curr;
	
	if(curr)
		curr->prev = block;
}

void Kernel::MemoryAllocators::Buddy::removeBlock(FreeBlock* block, size_t degree) {
	(block->prev ? block->prev->next : blocks[degree]) = block->next;

	if(block->next)
		block->next->prev = block->prev;
}

size_t Kernel::MemoryAllocators::Buddy::findFreeDegree(size_t degree) {
	for(size_t i = degree; i < BLOCK_SIZE_DEGRESS; i++)
		if(blocks[i] != nullptr)
			return i;

	return BLOCK_SIZE_DEGRESS;
}

void Kernel::MemoryAllocators::Buddy::recursiveSplit(size_t from, size_t to) {
	auto block = blocks[from];
	removeBlock(block, from);
	
	for(; from > to; from--) {
		auto buddy = block->getBuddy(from - 1);
		insertBlock(buddy, from - 1);
	}
}