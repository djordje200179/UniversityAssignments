#pragma once

#include "../syscall_c.h"
#include "Singleton.hpp"
#include "Thread.hpp"

namespace Kernel {
class Thread;

SINGLETON_CLASS(Timer)
public:
	void put(Thread* thread, time_t ticks);
	void tick();
private:
	Timer() = default;

	void update() { head->sleepingTicks--; }
	void release();

	Thread* head = nullptr;
};
}