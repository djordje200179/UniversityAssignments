#pragma once

#include "../../lib/hw.h"
#include "MemoryAllocators/Cache.hpp"

namespace Kernel {
class Thread {
// Misc
public:
	using Function = void (*)(void*);

	enum class State {
		CREATED, READY, RUNNING, BLOCKED
	};

	enum class Registers : uint8 {
		zero, ra, sp, gp, tp, t0, t1, t2,
		s0, s1, a0, a1, a2, a3, a4, a5,
		a6, a7, s2, s3, s4, s5, s6, s7,
		s8, s9, s10, s11, t3, t4, t5, t6
	};

	friend class Scheduler;
	friend class Timer;
	friend class Semaphore;

	CACHE_ALLOCATED(Thread);

// Static members
public:
	static void init();

	static void tick();
	static void dispatch();

	static Thread* getCurrent() { return current; }
private:
	static void wrapperFunction();

	static Thread* main;

	static Thread* current asm("current_thread");
	static size_t currentTimeLeft;

// Nonstatic members
public:
	Thread(uint64* stack, Function function, void* arg);
	~Thread();

	uint64 getReg(Registers reg) const { return context.registers[(uint8)reg]; }
	void setReg(Registers reg, uint64 value) { context.registers[(uint8)reg] = value; }

	uint32* getPC() const { return context.programCounter; }
	void setPC(uint32* value) { context.programCounter = value; }
private:
	Thread();

	uint64* stack;

	struct Context {
		uint32* programCounter;
		uint64 registers[32];
	} context;

	State state = State::CREATED;

	Function function;
	void* arg;

	Thread* next = nullptr;
	time_t sleepingTicks = 0;
};
}