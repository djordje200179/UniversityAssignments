#include "../h/Kernel/Thread.hpp"
#include "../h/Kernel/Trap.hpp"
#include "../h/syscall_c.h"

inline void registerInterrupts() {
	asm volatile("csrw stvec, %0" : : "r" (Kernel::Events::trap));
}

inline void startTimer() {
	asm volatile("csrs sstatus, 0b10");
}

void userMain() {

}

void main() {
	registerInterrupts();
	Kernel::Thread::init();
	startTimer();
	exit_sys();

    userMain();
}