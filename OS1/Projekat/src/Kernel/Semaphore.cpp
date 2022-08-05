#include "../../h/Kernel/Semaphore.hpp"
#include "../../h/Kernel/Scheduler.hpp"
#include "../../h/Kernel/Thread.hpp"

Kernel::Semaphore::~Semaphore() {
	auto& scheduler = Scheduler::getInstance();
	for (Thread* current = head; current; current = current->next) {
		current->setReg(Thread::Registers::a0, -1);
		scheduler.put(current);
	}
}

void Kernel::Semaphore::wait() {
	value--;
	if (value < 0)
		block();
}

void Kernel::Semaphore::signal() {
	value++;
	if (value <= 0)
		unblock();
}

void Kernel::Semaphore::block() {
	auto current = Thread::getCurrent();
	current->state = Thread::State::BLOCKED;

	tail = (tail ? tail->next : head) = current;
	current->next = nullptr;

	Thread::dispatch();
}

void Kernel::Semaphore::unblock() {
	auto first = head;

	head = head->next;
	if (!head)
		tail = nullptr;

	Scheduler::getInstance().put(first);
}