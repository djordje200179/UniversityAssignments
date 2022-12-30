#pragma once

#include "../../../lib/hw.h"

namespace Kernel {
namespace MemoryAllocators {
class Buddy {
public:
	Buddy(const Buddy&) = delete;
	Buddy& operator=(const Buddy&) = delete;

	static void initInstance(void* startAddress, size_t blocks);
	
	static Buddy& getInstance() {
		return instance;
	}
private:
	Buddy() = default;
	
	static Buddy instance;

public:
	void* allocate(size_t pages);
	void deallocate(void* ptr, size_t pages);
private:
	struct FreeBlock {
		FreeBlock* prev;
		FreeBlock* next;
		size_t degree;
		
		FreeBlock* getBuddy() const {
			return (FreeBlock*)((uint64)this ^ getSize());
		}

		size_t getSize() const {
			return 1UL << degree;
		}
	};
	
	void insertBlock(FreeBlock* block);
	void removeBlock(FreeBlock* block);
	bool containsBlock(FreeBlock* block, int degree);
	
	size_t findFreeDegree(size_t from);
	
	void recursiveJoin(FreeBlock* block);
	void recursiveSplit(size_t from, size_t to);
	
	static const size_t BLOCK_SIZE_DEGRESS = 30;
	
	FreeBlock* blocks[BLOCK_SIZE_DEGRESS];
};
}
}

#define BUDDY_ALLOCATED(T)                                                                      \
	static void* operator new(size_t pages) { return Buddy::getInstance().allocate(pages); }      \
	static void operator delete(void* ptr) { Buddy::getInstance().deallocate(ptr, sizeof(T)); }
