#pragma once

#include "../../../lib/hw.h"
#include "Buddy.hpp"
#include "Slab.hpp"

void userMain();
void* kmalloc(size_t);
void kfree(const void*);

namespace Kernel {
namespace MemoryAllocators {
class Cache {
// Misc
public:
	friend void ::userMain();
	friend void* ::kmalloc(size_t);
	friend void ::kfree(const void*);
	
	static void* operator new(size_t size) { return &cachesBlock[cachesCount++]; }
	static void operator delete(void* ptr);
	
// Static members
public:
	static void initCachesBlock();
	static void initBufferCaches();
	static Cache* getBufferCache(size_t degree);
private:
	static const size_t MIN_BUFFER_CACHE_DEGREE = 5;
	static const size_t MAX_BUFFER_CACHE_DEGREE = 17;
	
	static Cache* cachesBlock;
	static unsigned int cachesCount;
	static Cache* cachesHead;
	static Cache* bufferCaches[MAX_BUFFER_CACHE_DEGREE - MIN_BUFFER_CACHE_DEGREE + 1];
	
// Nonstatic members
public:
	Cache(size_t typeSize, const char* name, Slab::OBJ_FUN ctor, Slab::OBJ_FUN dtor) :
		typeSize(typeSize), name(name), ctor(ctor), dtor(dtor) {
		nextCache = cachesHead;
		cachesHead = this;
	}

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