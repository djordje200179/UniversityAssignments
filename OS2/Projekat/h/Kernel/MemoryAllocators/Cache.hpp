#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

namespace Kernel {
namespace MemoryAllocators {
class Cache {
// Misc
public:
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	
// Static members
public:
	static void initCachesBlock();
private:
	static void* cachesBlock;
	static unsigned int cachesCount;
	static Cache* cachesHead;
	
// Nonstatic members
public:
	Cache(size_t typeSize, const char* name, Slab::OBJ_FUN ctor, Slab::OBJ_FUN dtor) :
		typeSize(typeSize), name(name), ctor(ctor), dtor(dtor) {}

	void* allocate();
	bool deallocate(void* ptr);
	size_t shrink();
	void printInfo();
	int printError();
private:
	size_t typeSize;
	const char* name;
	Slab::OBJ_FUN ctor, dtor;

	Cache* nextCache = nullptr;

	Slab* fullSlabsHead = nullptr;
	Slab* partialSlabsHead = nullptr;
	Slab* emptySlabsHead = nullptr;

	bool canShrink = true;
};
}
}