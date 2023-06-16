#include "common/relocation.hpp"
#include <fstream>

void relocation::serialize(std::ofstream& os) const {
	os.write((const char*)(&offset), sizeof(offset));
	os.write((const char*)(&symbol), sizeof(symbol));
	os.write((const char*)(&addend), sizeof(addend));
}

void relocation::deserialize(std::ifstream& is) {
	
}

std::ostream& operator<<(std::ostream& os, const relocation& relocation) {
	os << "At " << relocation.offset << " write symbol " << relocation.symbol
	   << " + " << relocation.addend;

	return os;
}