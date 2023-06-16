#pragma once

#include <ostream>

struct relocation {
	uint64_t offset;

	uint64_t symbol;
	uint64_t addend;

	void serialize(std::ofstream& os) const;
	void deserialize(std::ifstream& is);

	friend std::ostream& operator<<(std::ostream& os, const relocation& relocation);
};