#pragma once

#include "slab.h"

#define CACHE_ALLOCATED(T) \
	public: \
		static void* operator new(size_t size) { return kmem_cache_alloc(getCache()); } \
		static void operator delete(void* ptr) { kmem_cache_free(getCache(), ptr); } \
	private: \
		static kmem_cache_t* getCache() { \
			static kmem_cache_t* cache = kmem_cache_create(#T, sizeof(T), 0, 0); \
			return cache; \
		}
