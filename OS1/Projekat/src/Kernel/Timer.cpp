#include "../../h/Kernel/Timer.hpp"
#include "../../h/Kernel/Scheduler.hpp"

void Kernel::Timer::put(Thread* thread, time_t ticks) {
	if(ticks == 0)
		return;

	thread->state = Thread::State::BLOCKED;

	Thread* prev_node = nullptr;
	for(auto curr_node = head; curr_node; prev_node = curr_node, curr_node = curr_node->next) {
		if(ticks <= curr_node->sleepingTicks)
			break;

		ticks -= curr_node->sleepingTicks;
	}

	Thread* next_node;
	if(prev_node) {
		next_node = prev_node->next;
		prev_node->next = thread;
	} else {
		next_node = head;
		head = thread;
	}
	thread->next = next_node;

	thread->sleepingTicks = ticks;
	if(next_node)
		next_node->sleepingTicks -= ticks;

	Thread::dispatch();
}

void Kernel::Timer::tick() {
	if(!head)
		return;

	update();
	release();
}

void Kernel::Timer::release() {
	auto& scheduler = Scheduler::getInstance();

	while(head && head->sleepingTicks == 0) {
		auto first = head;
		head = head->next;
		first->next = nullptr;
		scheduler.put(first);
	}
}