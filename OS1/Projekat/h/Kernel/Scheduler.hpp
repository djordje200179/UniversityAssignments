#pragma once

#include "Singleton.hpp"

namespace Kernel {
class Thread;

SINGLETON_CLASS(Scheduler)
public:
	void put(Thread* thread);
	Thread* get();
private:
	Scheduler() = default;

	Thread* head = nullptr;
	Thread* tail = nullptr;

	Thread* idleThread = nullptr;
};
}