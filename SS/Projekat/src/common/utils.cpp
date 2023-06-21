#include "common/utils.hpp"
#include <sstream>
#include <iomanip>
#include <string>

std::string to_hex(uint32_t num) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(8) << std::hex << num << std::dec;

	return stream.str();
}