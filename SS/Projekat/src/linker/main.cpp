#include "linker/args.hpp"
#include "common/executable_file.hpp"
#include "common/object_file.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<object_file> read_files(std::vector<std::string> paths) {
	std::vector<object_file> object_files(paths.size());

	for (size_t i = 0; i < paths.size(); i++) {
		std::ifstream file(paths[i], std::ios::binary);

		object_files[i].deserialize(file);

		file.close();
	}

	return object_files;
}

object_file link(std::vector<object_file> object_files);

int main(int argc, char** argv) {
	try {
		auto args = read_args(argc, argv);
		auto object_files = read_files(args.input_file_paths);

		auto linked_file = link(object_files);

		// std::cout << "Linked" << std::endl;
		// std::cout << linked_file << std::endl;

		std::ofstream output_file(args.output_file_path, std::ios::binary);
		if (args.relocatable)
			linked_file.serialize(output_file);
		else if(args.hex) {
			executable_file executable_file(linked_file, args.places);
			executable_file.serialize(output_file);
		}
		output_file.close(); 
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}