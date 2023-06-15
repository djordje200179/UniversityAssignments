#include "common/section.hpp"
#include "common/instruction.hpp"

void section::append(const void* data, size_t size) {
	auto null_data = !data;

	auto byte_date = data ? reinterpret_cast<const uint8_t*>(data) : new uint8_t[size];

	content.insert(content.end(), byte_date, byte_date + size);

	if (null_data)
		delete[] byte_date;
}

void section::append_literal(uint32_t data) {
	auto jump_instruction = instruction::make_jump(instruction::jump_mode::UNCONDITIONAL, false, 14, 14, 0, 4);
	
	append(&jump_instruction, sizeof(jump_instruction));
	append(&data, 4);
}

std::ostream& operator<<(std::ostream& os, const section& section) {
	os << "Section \"" << section.name << "\"\n";
	for (const auto& relocation : section.relocation_table)
		os << relocation << "\n";

	return os;
}