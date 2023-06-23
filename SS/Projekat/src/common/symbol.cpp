#include "common/symbol.hpp"
#include <algorithm>
#include <iomanip>
#include <fstream>

void symbol::serialize(std::ofstream& os) const {
	os.write((const char*)(&type), sizeof(type));
	os.write(name.data(), name.size() + 1);
	os.write((const char*)(&value), sizeof(value));
	os.write((const char*)(&section), sizeof(section));
	os.write((const char*)(&global), sizeof(global));
}

void symbol::deserialize(std::ifstream& is) {
	is.read((char*)(&type), sizeof(type));
	std::getline(is, name, '\0');
	is.read((char*)(&value), sizeof(value));
	is.read((char*)(&section), sizeof(section));
	is.read((char*)(&global), sizeof(global));
}

std::ostream& operator<<(std::ostream& os, const symbol& symbol) {
	if (symbol.name.size() > 7)
		os << symbol.name.substr(0, 6) << "~";
	else
		os << std::setw(7) << std::left << symbol.name;
	os << '\t';

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

const symbol* symbol_table::find(const std::string& name) const {
	return const_cast<symbol_table*>(this)->find(name);
}

void symbol_table::serialize(std::ofstream& os) const {
	auto symbols_count = this->size();
	os.write((const char*)(&symbols_count), sizeof(symbols_count));
	for (const auto& symbol : *this)
		symbol.serialize(os);
}

void symbol_table::deserialize(std::ifstream& is) {
	size_t symbols_count = 0;
	is.read((char*)(&symbols_count), sizeof(symbols_count));
	this->resize(symbols_count);
	for (auto& symbol : *this)
		symbol.deserialize(is);
}

std::ostream& operator<<(std::ostream& os, const symbol_table& table) {
	os << "Num\tName\tType\tValue\tSection\tBind\n";
	for (size_t i = 0; i < table.size(); i++) {
		os << i << "\t" << table[i] << "\n";
	}

	return os;
}