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

	section() = default;
	section(symbol section_symbol) { name = section_symbol.name; }

	void append(const void* data, size_t size);
	void append_literal(const uint32_t data);

	size_t size() const { return content.size(); }

	void serialize(std::ofstream& os) const;
	void deserialize(std::ifstream& is);

	friend std::ostream& operator<<(std::ostream& os, const section& section);
};