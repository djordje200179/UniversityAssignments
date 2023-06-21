#include "common/relocation.hpp"
#include "common/utils.hpp"
#include <fstream>

void relocation::serialize(std::ofstream& os) const {
	os.write((const char*)(&offset), sizeof(offset));
	os.write((const char*)(&symbol), sizeof(symbol));
	os.write((const char*)(&addend), sizeof(addend));
}

void relocation::deserialize(std::ifstream& is) {
	is.read((char*)(&offset), sizeof(offset));
	is.read((char*)(&symbol), sizeof(symbol));
	is.read((char*)(&addend), sizeof(addend));
}

std::ostream& operator<<(std::ostream& os, const relocation& relocation) {
	os << "At " << to_hex(relocation.offset) << " write symbol " << relocation.symbol << " + " << relocation.addend;

	return os;
}