#include "../h/Kernel/MemoryAllocators/Buddy.hpp"
#include "../h/Kernel/MemoryAllocators/Cache.hpp"
#include "../h/slab.h"

using namespace Kernel::MemoryAllocators;

void kmem_init(void* space, int block_num) {
	Buddy::initInstance(space, block_num);
	Cache::initCachesBlock();
}

kmem_cache_t* kmem_cache_create(const char* name, size_t size, void(*ctor)(void*), void(*dtor)(void*)) {
	return (kmem_cache_t*)(new Cache(size, name, ctor, dtor));
}

int kmem_cache_shrink(kmem_cache_t* cachep) {
	return ((Cache*)cachep)->shrink();
}

void* kmem_cache_alloc(kmem_cache_t* cachep) {
	return ((Cache*)cachep)->allocate();
}

void kmem_cache_free(kmem_cache_t* cachep, void* objp) {
	((Cache*)cachep)->deallocate(objp);
}

void kmem_cache_destroy(kmem_cache_t* cachep) {
	delete (Cache*)cachep;
}

void kmem_cache_info(kmem_cache_t* cachep) {
	((Cache*)cachep)->printInfo();
}

int kmem_cache_error(kmem_cache_t* cachep) {
	return ((Cache*)cachep)->printError();
}
