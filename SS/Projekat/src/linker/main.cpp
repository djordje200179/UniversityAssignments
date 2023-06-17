#include "linker/args.hpp"
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

int main(int argc, char** argv) {
	try {
		auto args = read_args(argc, argv);
		auto object_files = read_files(args.input_file_paths);

		for (const auto& file : object_files)
			std::cout << file << std::endl;

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}