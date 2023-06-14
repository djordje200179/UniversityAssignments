#include "common/section.hpp"

section::section(symbol section_symbol) {
	name = section_symbol.name;
}

std::ostream& operator<<(std::ostream& os, const section& section) {
	os << "Section \"" << section.name << "\"\n";
	for (const auto& relocation : section.relocation_table)
		os << relocation << "\n";

	return os;
}