#pragma once

#include <ostream>
#include <vector>
#include "section.hpp"
#include "symbol.hpp"

struct object_file {
	symbol_table symbols;
	std::vector<section> sections;

	void serialize(std::ofstream& os) const;
	void deserialize(std::ifstream& is);

	friend std::ostream& operator<<(std::ostream& os, const object_file& object_file);
};