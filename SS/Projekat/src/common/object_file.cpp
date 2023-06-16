#include "common/object_file.hpp"

std::ostream& operator<<(std::ostream& os, const object_file& object_file) {
	os << "-- Symbol table --" << std::endl;
	os << object_file.symbols << std::endl;

	os << "-- Sections --" << std::endl;
	for (const auto& section : object_file.sections)
		os << section << std::endl;
	
	return os;
}