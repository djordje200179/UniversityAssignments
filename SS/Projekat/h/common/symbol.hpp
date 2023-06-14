#pragma once

#include <ostream>
#include <string>
#include <vector>

constexpr size_t SECTION_ABS = 0;
constexpr size_t SECTION_UNDEF = static_cast<size_t>(-1);

struct symbol {
	enum class type { NOTYPE, SECTION } type;
	std::string name;
	uint64_t value;
	uint64_t section;
	bool global;

	friend std::ostream& operator<<(std::ostream& os, const symbol& symbol);
};

struct symbol_table : public std::vector<symbol> {
	symbol* find(const std::string& name);
	friend std::ostream& operator<<(std::ostream& os,
									const symbol_table& table);
};