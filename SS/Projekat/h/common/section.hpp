#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "relocation.hpp"
#include "symbol.hpp"

struct section {
	std::string name;
	std::vector<uint8_t> content;
	std::vector<relocation> relocation_table;

	section(symbol section_symbol);
	friend std::ostream& operator<<(std::ostream& os, const section& section);
};