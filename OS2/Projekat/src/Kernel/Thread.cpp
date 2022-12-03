#include "../../h/Kernel/Thread.hpp"
#include "../../h/Kernel/Scheduler.hpp"
#include "../../h/syscall_c.h"

Kernel::MemoryAllocators::Cache* Kernel::Thread::cache = nullptr;

Kernel::Thread* Kernel::Thread::main;
Kernel::Thread* Kernel::Thread::current;
size_t Kernel::Thread::currentTimeLeft;

Kernel::Thread::Thread(uint64* stack, Function function, void* arg) :
		stack(stack), function(function), arg(arg) {
	for (int i = 0; i < 32; i++)
		setReg((Registers)i, 0);
	setReg(Registers::sp, (uint64)&stack[DEFAULT_STACK_SIZE]);
	setPC((uint32*)wrapperFunction);
}

Kernel::Thread::Thread() : stack(nullptr) {

}

Kernel::Thread::~Thread() {
	if (stack)
		MemoryAllocators::Heap::getInstance().deallocate(stack);

	if (this == current) {
		current = nullptr;
		Thread::dispatch();
	}
}

void Kernel::Thread::init() {
	current = main = new Thread();
	currentTimeLeft = DEFAULT_TIME_SLICE;
}

void Kernel::Thread::tick() {
	currentTimeLeft--;
	if (currentTimeLeft == 0)
		dispatch();
}

void Kernel::Thread::wrapperFunction() {
	current->function(current->arg);
	thread_exit();
}

void Kernel::Thread::dispatch() {
	auto oldThread = current;

	if (oldThread && oldThread->state != State::BLOCKED)
		Scheduler::getInstance().put(oldThread);

	auto newThread = Scheduler::getInstance().get();

	current = newThread;
	current->state = State::RUNNING;
	currentTimeLeft = DEFAULT_TIME_SLICE;
}