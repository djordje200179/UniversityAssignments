#include "common/object_file.hpp"
#include <fstream>
#include <algorithm>

void object_file::serialize(std::ofstream& os) const {
	symbols.serialize(os);

	auto sections_count = sections.size();
	os.write((const char*)(&sections_count), sizeof(sections_count));
	for (const auto& section : sections)
		section.serialize(os);
}

void object_file::deserialize(std::ifstream& is) {
	symbols.deserialize(is);

	size_t sections_count = 0;
	is.read((char*)(&sections_count), sizeof(sections_count));
	sections.resize(sections_count);
	for (auto& section : sections)
		section.deserialize(is);
}

std::ostream& operator<<(std::ostream& os, const object_file& object_file) {
	os << "-- Symbol table --" << std::endl;
	os << object_file.symbols << std::endl;

	os << "-- Sections --" << std::endl;
	for (const auto& section : object_file.sections)
		os << section << std::endl;
	
	return os;
}