#include "linker/args.hpp"
#include <iostream>
#include <sstream>

args read_args(int argc, char** argv) {
	args args;
	args.output_file_path = "a.out";

	for(int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-o") {
			if (i + 1 >= argc) {
				// TODO: Convert to try-catch
				std::cerr << "Error: -o option requires one argument." << std::endl;
				exit(1);
			}

			args.output_file_path = argv[i + 1];
			i++;
		} else if (arg.starts_with("-place")) {
			auto place_arg = arg.substr(7);
			auto delimeter_index = place_arg.find("@");
			if (delimeter_index == std::string::npos) {
				// TODO: Convert to try-catch
				std::cerr << "Error: -place option requires one argument." << std::endl;
				exit(1);
			}

			auto section_name = place_arg.substr(0, delimeter_index);

			uint32_t address;
			std::stringstream ss;
			ss << std::hex << place_arg.substr(delimeter_index + 1);
			ss >> address;

			args.places.push_back(place{
				.section_name = section_name,
				.address = address
			});
		} else if (arg == "-hex")
			args.hex = true;
		else if (arg == "-relocatable")
			args.relocatable = true;
		else {
			args.input_file_paths.push_back(arg);
		}
	}
	
	return args;
}