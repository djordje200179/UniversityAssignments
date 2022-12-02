#include "../../../h/Kernel/MemoryAllocators/Buddy.hpp"

Kernel::MemoryAllocators::Buddy::Buddy() {
	auto endAddress = (void*)((uint64)HEAP_END_ADDR - ((uint64)HEAP_END_ADDR >> 3));
}