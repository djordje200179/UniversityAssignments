#include "../../h/Kernel/Scheduler.hpp"
#include "../../h/Kernel/Thread.hpp"

void Kernel::Scheduler::put(Thread* thread) {
	if (thread == idleThread)
		return;

	thread->state = Thread::State::READY;
	tail = (tail ? tail->next : head) = thread;
}

Kernel::Thread* Kernel::Scheduler::get() {
	if (head) {
		Thread* ret = head;

		head = head->next;
		if (!head)
			tail = nullptr;

		ret->next = nullptr;
		return ret;
	} else {
		if (!idleThread)
			idleThread = new Thread(
					(uint64*)MemoryAllocators::Heap::getInstance().allocateBytes(DEFAULT_STACK_SIZE),
					[](void*) { while (true); },
					nullptr
			);

		return idleThread;
	}
}