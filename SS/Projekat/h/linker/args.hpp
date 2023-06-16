#pragma once

#include <string>
#include <vector>

struct place {
	std::string symbol;
	uint32_t address;
};

struct args {
	std::vector<std::string> input_file_paths;

	std::vector<place> places;

	std::string output_file_path;

	bool print_hex;
	bool relocatable;
};

args read_args(int argc, char** argv);