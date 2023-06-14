#include "common/relocation.hpp"

std::ostream& operator<<(std::ostream& os, const relocation& relocation) {
	os << "At " << relocation.offset << " write symbol " << relocation.symbol
	   << " + " << relocation.addend;

	return os;
}