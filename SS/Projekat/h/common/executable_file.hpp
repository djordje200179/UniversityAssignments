#pragma once

#include "common/object_file.hpp"
#include "linker/args.hpp"
#include <map>

struct executable_file : public std::map<uint32_t, uint8_t> {
	std::map<std::string, std::pair<uint32_t, uint32_t>> section_addresses;
	std::map<std::string, uint32_t> symbol_addresses;

	executable_file() = default;
	executable_file(const object_file& object_file, const std::vector<place>& places);

	void serialize(std::ofstream& file);
	void deserialize(std::ifstream& file);

	friend std::ostream& operator<<(std::ostream& stream, const executable_file& executable_file);
private:
	void setup_section_addresses(const object_file& object_file, const std::vector<place>& places);
	void setup_symbol_addresses(const object_file& object_file);
	
	size_t free_address = 0;
};