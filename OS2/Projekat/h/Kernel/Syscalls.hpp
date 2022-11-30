#pragma once

#include "../../lib/hw.h"

namespace Kernel {
class Syscalls {
public:
	Syscalls(const Syscalls&) = delete;
	Syscalls& operator=(const Syscalls&) = delete;

	static Syscalls& getInstance() {
		static Syscalls instance;
		return instance;
	}

public:
	enum Type : uint64 {
		MEM_ALLOC		= 0x01,
		MEM_FREE		= 0x02,

		THREAD_CREATE	= 0x11,
		THREAD_EXIT		= 0x12,
		THREAD_DISPATCH	= 0x13,
		THREAD_INIT		= 0x14,
		THREAD_START	= 0x15,

		SEM_OPEN		= 0x21,
		SEM_CLOSE		= 0x22,
		SEM_WAIT		= 0x23,
		SEM_SIGNAL		= 0x24,

		TIME_SLEEP		= 0x31,

		PUTC 			= 0x41,
		GETC 			= 0x42,

		EXIT_SYS 		= 0x51,
	};

	void handle();
private:
	Syscalls();
	
	void(*functions[0x100])();
};
}