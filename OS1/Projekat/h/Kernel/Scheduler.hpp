#pragma once

namespace Kernel {
class Thread;

class Scheduler {
public:
	Scheduler(const Scheduler&) = delete;
	Scheduler& operator=(const Scheduler&) = delete;

	static Scheduler& getInstance() {
		static Scheduler instance;
		return instance;
	}
private:
	Scheduler() = default;

public:
	void put(Thread* thread);
	Thread* get();
private:
	Thread* head = nullptr;
	Thread* tail = nullptr;

	Thread* idleThread = nullptr;
};
}