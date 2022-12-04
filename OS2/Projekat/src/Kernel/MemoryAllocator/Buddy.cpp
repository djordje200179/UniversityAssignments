#include "../../../h/Kernel/MemoryAllocators/Buddy.hpp"

Kernel::MemoryAllocators::Buddy::Buddy() {
	auto endAddress = (void*)((uint64)HEAP_END_ADDR - ((uint64)HEAP_END_ADDR >> 3));
	((char*)endAddress + 1);
}

void* Kernel::MemoryAllocators::Buddy::allocate(size_t size) {
	return nullptr;
}

void Kernel::MemoryAllocators::Buddy::deallocate(void* ptr, size_t size) {

}