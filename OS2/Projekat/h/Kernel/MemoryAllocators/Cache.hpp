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
	static const size_t NAME_BUFFER_SIZE = 16;
	
	static Cache* cachesBlock;
	static unsigned int cachesCount;
	static Cache* cachesHead;
	static Cache* bufferCaches[MAX_BUFFER_CACHE_DEGREE - MIN_BUFFER_CACHE_DEGREE + 1];
	
// Nonstatic members
public:
	Cache(size_t typeSize, const char* name, Slab::OBJ_FUN ctor, Slab::OBJ_FUN dtor) :
		typeSize(typeSize), ctor(ctor), dtor(dtor) {
		for (size_t i = 0; i < NAME_BUFFER_SIZE; i++) {
			this->name[i] = name[i];
			if (!name[i])
				break;
		}
		
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
	char name[NAME_BUFFER_SIZE];
	Slab::OBJ_FUN ctor, dtor;

	Cache* nextCache = nullptr;

	Slab* fullSlabsHead = nullptr;
	Slab* partialSlabsHead = nullptr;
	Slab* emptySlabsHead = nullptr;

	bool canShrink = true;
};
}
}