#include "../../h/Kernel/Thread.hpp"
#include "../../h/Kernel/Scheduler.hpp"
#include "../../h/Kernel/Semaphore.hpp"
#include "../../h/Kernel/Timer.hpp"
#include "../../h/Kernel/Console.hpp"
#include "../../h/Kernel/Syscalls.hpp"
#include "../../h/Kernel/MemoryAllocators/Heap.hpp"

#define GET_PARAM(index, type) (type)Kernel::Thread::getCurrent()->getReg(Kernel::Thread::Registers::a ## index)
#define SET_RET_VAL(value) Kernel::Thread::getCurrent()->setReg(Kernel::Thread::Registers::a0, (uint64)value)

namespace Kernel {
static void mem_alloc() {
	auto requestedBlocks = GET_PARAM(1, size_t);

	void* result = MemoryAllocators::Heap::getInstance().allocate(requestedBlocks);
	SET_RET_VAL(result);
}

static void mem_free() {
	auto ptr = GET_PARAM(1, void*);

	MemoryAllocators::Heap::getInstance().deallocate(ptr);
	SET_RET_VAL(0);
}

static void exit_sys() {
	uint64 sstatus;
	asm volatile ("csrr %0, sstatus" : "=r" (sstatus));
	sstatus &= ~0x100;
	asm volatile ("csrw sstatus, %0" : : "r" (sstatus));
}

static void thread_init() {
	auto handle = GET_PARAM(1, Thread**);
	auto startRoutine = GET_PARAM(2, Thread::Function);
	auto arg = GET_PARAM(3, void*);
	auto stack = GET_PARAM(4, uint64*);

	*handle = (stack ? new Thread(stack, startRoutine, arg) : nullptr);
}

static void thread_start(bool just_created) {
	Thread* thread;
	if (just_created) {
		auto handle = GET_PARAM(1, Thread**);
		thread = *handle;
	} else
		thread = GET_PARAM(1, Thread*);

	if (thread) {
		Scheduler::getInstance().put(thread);
		SET_RET_VAL(0);
	} else
		SET_RET_VAL(-1);
}

static void thread_create() {
	thread_init();
	thread_start(true);
}

static void thread_exit() {
	auto current = Thread::getCurrent();

	SET_RET_VAL(-1);
	delete current;
}

static void sem_open() {
	auto handle = GET_PARAM(1, Semaphore**);
	auto init = GET_PARAM(2, unsigned);

	if ((*handle = new Semaphore(init)) == nullptr)
		SET_RET_VAL(-1);
	SET_RET_VAL(0);
}

static void sem_close() {
	auto sem = GET_PARAM(1, Semaphore*);

	SET_RET_VAL(0);
	delete sem;
}

static void sem_wait() {
	auto sem = GET_PARAM(1, Semaphore*);

	SET_RET_VAL(0);
	sem->wait();
}

static void sem_signal() {
	auto sem = GET_PARAM(1, Semaphore*);

	SET_RET_VAL(0);
	sem->signal();
}

static void time_sleep() {
	auto ticks = GET_PARAM(1, time_t);
	auto current = Thread::getCurrent();

	SET_RET_VAL(0);
	Timer::getInstance().put(current, ticks);
}

static void putc() {
	auto c = GET_PARAM(1, char);
	Console::getInstance().writeChar(c);
}

static void getc() {
	auto c = Console::getInstance().readChar();
	SET_RET_VAL(c);
}

static void incrementPC() {
	auto thread = Thread::getCurrent();
	auto pc = thread->getPC();
	pc++;
	thread->setPC(pc);
}

void Syscalls::handle() {
	incrementPC();

	auto type = GET_PARAM(0, Type);
	functions[type]();
}

Syscalls::Syscalls() {
	functions[MEM_ALLOC] = mem_alloc;
	functions[MEM_FREE] = mem_free;

	functions[THREAD_CREATE] = thread_create;
	functions[THREAD_INIT] = thread_init;
	functions[THREAD_START] = []() { thread_start(false); };
	functions[THREAD_EXIT] = thread_exit;
	functions[THREAD_DISPATCH] = Thread::dispatch;

	functions[SEM_OPEN] = sem_open;
	functions[SEM_CLOSE] = sem_close;
	functions[SEM_WAIT] = sem_wait;
	functions[SEM_SIGNAL] = sem_signal;

	functions[TIME_SLEEP] = time_sleep;

	functions[PUTC] = putc;
	functions[GETC] = getc;

	functions[EXIT_SYS] = exit_sys;
}
}