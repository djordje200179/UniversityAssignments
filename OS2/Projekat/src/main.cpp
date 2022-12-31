#include "../h/Kernel/Thread.hpp"
#include "../h/Kernel/Trap.hpp"
#include "../h/syscall_c.h"
#include "../h/slab.h"
#include "../h/Kernel/MemoryAllocators/Cache.hpp"

inline void registerInterrupts() {
	asm volatile("csrw stvec, %0" : : "r" (Kernel::Events::trap));
}

inline void startTimer() {
	asm volatile("csrs sstatus, 0b10");
}

void userMain() {
	using namespace Kernel::MemoryAllocators;
	
	for (auto currCache = Cache::cachesHead; currCache; currCache = currCache->nextCache)
		currCache->printInfo();

	time_sleep(1);

	for (auto currCache = Cache::cachesHead; currCache; currCache = currCache->nextCache)
		currCache->printInfo();
}

void main() {
	registerInterrupts();
	kmem_init((void*)0x87000000, (0x88000000 - 0x87000000) / BLOCK_SIZE);
	
	Kernel::Thread::init();
	startTimer();
	exit_sys();

	userMain();
}