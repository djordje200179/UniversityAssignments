#include "../h/syscall_c.h"
#include "../h/Kernel/MemoryAllocators/Heap.hpp"
#include "../h/Kernel/Console.hpp"
#include "../h/Kernel/Syscalls.hpp"

#define SET_PARAM(index, value) asm volatile ("mv a" #index ", %0" : : "r" (value))

#define SYSCALL(syscall) {            				\
    asm volatile ("li a0, %0" : : "i" (syscall));	\
    asm volatile ("ecall");  						\
}

#define SET_RET_VAL(type) { 						\
	type ret_value;   	                 			\
    asm volatile ("mv %0, a0" : "=r" (ret_value)); 	\
	return ret_value; 								\
}

#define SYSCALL_WITH_RET(syscall, type) { SYSCALL(syscall); SET_RET_VAL(type); }

using Kernel::Syscalls;

void* mem_alloc(size_t size) {
	size = Kernel::MemoryAllocators::Heap::calculateBlocks(size);

	SET_PARAM(1, size);

	SYSCALL_WITH_RET(Syscalls::MEM_ALLOC, void*);
}

int mem_free(void* ptr) {
	SET_PARAM(1, ptr);

	SYSCALL_WITH_RET(Syscalls::MEM_FREE, int);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));

	SET_PARAM(4, stack_space);
	SET_PARAM(3, arg);
	SET_PARAM(2, start_routine);
	SET_PARAM(1, handle);

	SYSCALL_WITH_RET(Syscalls::THREAD_CREATE, int);
}

void thread_init(thread_t* handle, void(*start_routine)(void*), void* arg) {
	void* stack_space = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));

	SET_PARAM(4, stack_space);
	SET_PARAM(3, arg);
	SET_PARAM(2, start_routine);
	SET_PARAM(1, handle);

	SYSCALL(Syscalls::THREAD_INIT);
}

int thread_start(thread_t handle) {
	SET_PARAM(1, handle);

	SYSCALL_WITH_RET(Syscalls::THREAD_START, int);
}

int thread_exit() {
	SYSCALL_WITH_RET(Syscalls::THREAD_EXIT, int);
}

void thread_dispatch() {
	SYSCALL(Syscalls::THREAD_DISPATCH);
}

int sem_open(sem_t* handle, unsigned init) {
	SET_PARAM(2, init);
	SET_PARAM(1, handle);

	SYSCALL_WITH_RET(Syscalls::SEM_OPEN, int);
}

int sem_close(sem_t handle) {
	SET_PARAM(1, handle);

	SYSCALL_WITH_RET(Syscalls::SEM_CLOSE, int);
}

int sem_wait(sem_t id) {
	SET_PARAM(1, id);

	SYSCALL_WITH_RET(Syscalls::SEM_WAIT, int);
}

int sem_signal(sem_t id) {
	SET_PARAM(1, id);

	SYSCALL_WITH_RET(Syscalls::SEM_SIGNAL, int);
}

int time_sleep(time_t ticks) {
	SET_PARAM(1, ticks);

	SYSCALL_WITH_RET(Syscalls::TIME_SLEEP, int);
}

char getc() {
	Kernel::Console::getInstance().waitForInputReady();

	SYSCALL_WITH_RET(Syscalls::GETC, char);
}

void putc(char c) {
	Kernel::Console::getInstance().waitForOutputReady();

	SET_PARAM(1, c);

	SYSCALL(Syscalls::PUTC);
}

void exit_sys() {
	SYSCALL(Syscalls::EXIT_SYS);
}