#include "common/section.hpp"

void section::append(const uint8_t* data, size_t size) {
	auto null_data = false;
	if (!data) {
		data = new uint8_t[size];
		null_data = true;
	}

	content.insert(content.end(), data, data + size);

	if (null_data)
		delete[] data;
}

std::ostream& operator<<(std::ostream& os, const section& section) {
	os << "Section \"" << section.name << "\"\n";
	for (const auto& relocation : section.relocation_table)
		os << relocation << "\n";

	return os;
}