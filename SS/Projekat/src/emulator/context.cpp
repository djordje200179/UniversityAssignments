#include "emulator/context.hpp"
#include "common/utils.hpp"
#include <iostream>

uint32_t context::read_from(uint32_t address) const {
	uint32_t word = 0;

	for (int i = 0; i < 4; i++)
		if (memory.contains(address + i))
			word |= memory.at(address + i) << (i * 8);

	return word;
}


instruction context::read_instruction() {
	auto raw_bytes = read_from(pc);
	auto inst_bytes = *(instruction*)&raw_bytes;

	pc += 4;

	return inst_bytes; 
}

void context::write_to(uint32_t address, uint32_t value) {
	if (address == 0xFFFFFF00)
		std::cout << (char)value << std::flush;

	for (int i = 0; i < 4; i++)
		memory[address + i] = (value >> (i * 8));
}

std::ostream& operator<<(std::ostream& os, const context& context) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			auto reg = i * 4 + j;

			if (reg < 10)
				os << ' ';
			os << 'r' << reg << '=' << to_hex(context.gprs[reg]) << '\t';
		}
		os << std::endl;
	}

	return os;
}