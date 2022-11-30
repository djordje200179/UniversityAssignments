#pragma once

#include "../syscall_c.h"
#include "Thread.hpp"

namespace Kernel {
class Thread;

class Timer {
public:
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	static Timer& getInstance() {
		static Timer instance;
		return instance;
	}
private:
	Timer() = default;

public:
	void put(Thread* thread, time_t ticks);
	void tick();
private:
	void update() { head->sleepingTicks--; }
	void release();

	Thread* head = nullptr;
};
}