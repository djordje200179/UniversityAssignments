#include "../../h/Kernel/Trap.hpp"
#include "../../h/Kernel/Thread.hpp"
#include "../../h/Kernel/Timer.hpp"
#include "../../h/Kernel/Console.hpp"
#include "../../h/Kernel/Syscalls.hpp"

namespace Kernel {
static void hardwareInterrupt() {
	auto irq = plic_claim();
	if(irq == CONSOLE_IRQ)
		Console::getInstance().handleInterrupt();
	plic_complete(irq);
}

static void timerTick() {
	Timer::getInstance().tick();
	Thread::tick();

	asm volatile ("csrc sip, 0b10");
}

void Kernel::Events::handle() {
	Type type;
	asm volatile ("csrr %0, scause" : "=r" (type));

	switch(type) {
		case Type::ECALL_USER:
		case Type::ECALL_SYS:
			Syscalls::getInstance().handle();
			break;
		case Type::TIMER:
			timerTick();
			break;
		case Type::EXTERNAL:
			hardwareInterrupt();
			break;
		case Type::ERR_INSTR:
		case Type::ERR_READ:
		case Type::ERR_WRITE:
		default:
			volatile Type copy = type;
			if(copy == Type::ERR_INSTR);
			break;
	}
}
}