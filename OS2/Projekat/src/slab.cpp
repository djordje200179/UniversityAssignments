#include "../h/Kernel/MemoryAllocators/Buddy.hpp"
#include "../h/Kernel/MemoryAllocators/Cache.hpp"
#include "../h/slab.h"

using namespace Kernel::MemoryAllocators;

static uint64 clog2(uint64 num) {
	unsigned int result = 0;

	for (num--; num > 0; num >>= 1)
		result++;

	return result;
}

void kmem_init(void* space, int block_num) {
	Buddy::initInstance(space, block_num);
	Cache::initCachesBlock();
	Cache::initBigSlabsCache();
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

void* kmalloc(size_t size) {
	auto degree = clog2(size);
	if (degree < Cache::MIN_BUFFER_CACHE_DEGREE || degree > Cache::MAX_BUFFER_CACHE_DEGREE)
		return nullptr;

	auto cache = Cache::getBufferCache(degree);
	return cache->allocate();
}

void kfree(const void* objp) {
	for (size_t i = 0; i < Cache::MAX_BUFFER_CACHE_DEGREE - Cache::MIN_BUFFER_CACHE_DEGREE + 1; i++) {
		auto cache = Cache::bufferCaches[i];

		if (!cache)
			continue;
		
		if (cache->deallocate((void*)objp))
			return;
	}
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
