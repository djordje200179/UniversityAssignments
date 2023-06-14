#include "common/symbol.hpp"
#include <algorithm>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const symbol& symbol) {
	os << std::setw(7) << std::left << symbol.name << "\t";
	switch (symbol.type) {
		case symbol::type::NOTYPE:
			os << "NOTYP\t";
			break;
		case symbol::type::SECTION:
			os << "SCTN\t";
			break;
	}
	os << symbol.value << "\t";
	switch (symbol.section) {
		case SECTION_UNDEF:
			os << "UND\t";
			break;
		case SECTION_ABS:
			os << "ABS\t";
			break;
		default:
			os << symbol.section << "\t";
	}
	os << (symbol.global ? "GLOB" : "LOC");

	return os;
}

symbol* symbol_table::find(const std::string& name) {
	auto it = find_if(begin(), end(), [name](const symbol& symbol) {
		return symbol.name == name;
	});
	if (it == end())
		return nullptr;

	return std::addressof(*it);
}

std::ostream& operator<<(std::ostream& os, const symbol_table& table) {
	os << "Num\tName\tType\tValue\tSection\tBind\n";
	for (size_t i = 0; i < table.size(); i++) {
		os << i << "\t" << table[i] << "\n";
	}

	return os;
}