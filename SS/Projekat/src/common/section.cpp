#include "common/section.hpp"
#include "common/instruction.hpp"
#include "common/utils.hpp"
#include <fstream>

void section::append(const void* data, size_t size) {
	auto null_data = !data;

	auto byte_date = data ? (const uint8_t*)data : new uint8_t[size];

	content.insert(content.end(), byte_date, byte_date + size);

	if (null_data)
		delete[] byte_date;
}

void section::append_literal(uint32_t data) {
	auto jump_instruction = instruction::make_jump(instruction::jump_mode::UNCONDITIONAL, false, 15, 15, 0, 4);
	
	append(&jump_instruction, sizeof(jump_instruction));
	append(&data, 4);
}

void section::serialize(std::ofstream& os) const {
	os.write((const char*)(name.data()), name.size() + 1);

	auto content_size = content.size();
	os.write((const char*)(&content_size), sizeof(content_size));
	os.write((const char*)(content.data()), content_size);

	auto relocations_count = relocation_table.size();
	os.write((const char*)(&relocations_count), sizeof(relocations_count));
	for (const auto& relocation : relocation_table)
		relocation.serialize(os);
}

void section::deserialize(std::ifstream& is) {
	// Read name until null terminator
	std::getline(is, name, '\0');

	size_t content_size = 0;
	is.read((char*)(&content_size), sizeof(content_size));
	content.resize(content_size);
	is.read((char*)(content.data()), content_size);

	size_t relocations_count = 0;
	is.read((char*)(&relocations_count), sizeof(relocations_count));
	relocation_table.resize(relocations_count);
	for (auto& relocation : relocation_table)
		relocation.deserialize(is);
}

std::ostream& operator<<(std::ostream& os, const section& section) {
	os << "Section \"" << section.name << '\"' << std::endl;
	os << "Content:" << std::endl;

	for(size_t i = 0; i < section.content.size(); i += 4) {
		os << to_hex(i) << ": ";

		for (size_t j = 0; j < 4 && i + j < section.content.size(); j++) {
			auto byte = section.content[i + j];
			if (byte < 0x10)
				os << '0';
			os << std::hex << (byte & 0xFF) << std::dec << ' ';
		}
		
		os << std::endl;
	}

	os << "Relocations:" << std::endl;
	for (const auto& relocation : section.relocation_table)
		os << relocation << std::endl;

	return os;
}