#include "common/executable_file.hpp"
#include <algorithm>
#include <fstream>
#include "common/utils.hpp"
#include <iostream>

executable_file::executable_file(const object_file& object_file, const std::vector<place>& places) {
	setup_section_addresses(object_file, places);

	for (const auto& section : object_file.sections) {
		auto section_address = section_addresses[section.name].first;

		for (size_t i = 0; i < section.content.size(); i++) {
			auto byte = section.content[i];

			insert(std::make_pair(section_address + i, byte));
		}

		for (const auto& relocation : section.relocation_table) {
			const auto& symbol = object_file.symbols[relocation.symbol];
			const auto& symbol_section_name = object_file.symbols[symbol.section].name; 

			auto symbol_address = section_addresses[symbol_section_name].first + symbol.value + relocation.addend;
			for (size_t i = 0; i < 4; i++)
				at(section_address + relocation.offset + i) = symbol_address >> (8 * i);
		}
	}
}

void executable_file::setup_section_addresses(const object_file& object_file, const std::vector<place>& places) {
	for (const auto& section : object_file.sections) {
		auto place_it = find_if(places.begin(), places.end(),
			[&section](const place& place) {
				return place.section_name == section.name;
			}
		);

		uint32_t section_address;
		if (place_it != places.end()) {
			section_address = place_it->address;

			if (section_address + section.content.size() > free_address)
				free_address = place_it->address + section.content.size();
		} else {
			section_address = free_address;
			free_address += section.content.size();
		}

		section_addresses[section.name] = std::make_pair(section_address, section_address + section.content.size());
	}
}

void executable_file::serialize(std::ofstream& file) {
	for (const auto& [address, byte] : *this) {
		file.write((const char*)(&address), sizeof(address));
		file.write((const char*)(&byte), sizeof(byte));
	}
}

void executable_file::deserialize(std::ifstream& file) {
	while (file.peek() != EOF) {
		uint32_t address;
		char byte;

		file.read((char*)(&address), sizeof(address));
		file.read((char*)(&byte), sizeof(byte));

		insert(std::make_pair(address, byte));
	}
}

std::ostream& operator<<(std::ostream& os, const executable_file& executable_file) {
	size_t last_address_group = -1;

	for (const auto& [address, byte] : executable_file) {
		if (address / 4 == last_address_group)
			continue;

		last_address_group = address / 4;

		uint32_t first_address = address / 4 * 4;
		os << to_hex(first_address) << ": ";

		for (size_t i = 0; i < 4; i++) {
			if (executable_file.contains(first_address + i)) {
				uint8_t byte = executable_file.at(first_address + i);
				if (byte < 0x10)
					os << '0';
				os << std::hex << (byte & 0xFF) << std::dec;
			} else
				os << "00";

			os << ' ';
		}
		os << std::endl;
	}

	return os;
}