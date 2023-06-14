#pragma once

#include <ostream>

struct relocation {
	uint64_t offset;

	uint64_t symbol;
	uint64_t addend;

	friend std::ostream& operator<<(std::ostream& os,
									const relocation& relocation);
};