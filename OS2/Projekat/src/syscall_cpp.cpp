#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"

void* operator new(size_t size) { return mem_alloc(size); }
void operator delete(void* ptr) { mem_free(ptr); }

struct PeriodicThreadData {
	PeriodicThread* thread;
	time_t period;
};

PeriodicThread::PeriodicThread(time_t period) : Thread(wrapper, new PeriodicThreadData{ this, period }) {}

void PeriodicThread::wrapper(void* arg) {
	auto data = (PeriodicThreadData*)arg;

	while (true) {
		sleep(data->period);
		data->thread->periodicActivation();
	}
}

void Console::puts(const char* str, bool newLine) {
	while (*str) putc(*str++);

	if (newLine)
		putc('\n');
}
