#pragma once

#include "../../lib/hw.h"

namespace Kernel {
namespace Events {
enum class Type : uint64 {
	ERR_INSTR	= 0x02,
	ERR_READ	= 0x05,
	ERR_WRITE	= 0x07,

	ECALL_USER	= 0x08,
	ECALL_SYS 	= 0x09,

	TIMER 		= 0x1UL << 63 | 0x01,
	EXTERNAL 	= 0x1UL << 63 | 0x09
};

void handle();
void trap();
}
}