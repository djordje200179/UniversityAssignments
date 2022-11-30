#pragma once

#include "../../../lib/hw.h"

namespace Kernel {
namespace MemoryAllocators {
class Buddy {
public:
	Buddy(const Buddy&) = delete;
	Buddy& operator=(const Buddy&) = delete;

	static Buddy& getInstance() {
		static Buddy instance;
		return instance;
	}
private:
	Buddy();

public:
	void* allocate(size_t size);
	void deallocate(void* ptr, size_t size);
};
}
}

#define BUDDY_ALLOCATED(type)                                                                       \
	static void* operator new(size_t size) { return Buddy::getInstance().allocate(size); }          \
	static void operator delete(void* ptr) { Buddy::getInstance().deallocate(ptr, sizeof(type)); }  \