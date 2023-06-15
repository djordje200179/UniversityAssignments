extern "C" {
#include "assembler/args.h"
#include "assembler/syntax.h"
}

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "assembler/exceptions.hpp"
#include "common/instruction.hpp"
#include "common/section.hpp"
#include "common/object_file.hpp"

extern "C" FILE* yyin;
extern "C" int yyparse(lines* ret_lines);

lines read_input_file(const std::string& input_file_path) {
	FILE* input_file = fopen(input_file_path.c_str(), "r");
	if (!input_file)
		throw file_not_found_error(input_file_path);

	yyin = input_file;

	struct lines lines = {NULL, 0};
	yyparse(&lines);

	fclose(input_file);

	return lines;
}

object_file assemble(lines lines);

int main(int argc, char** argv) {
	try {
		auto args = read_args(argc, argv);
		auto lines = read_input_file(args.input_file_path);

		auto object_file = assemble(lines);
		std::cout << object_file << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

extern "C" void yyerror(lines* ret_lines, const char* message) {
	lines_free(*ret_lines);

	extern int line_num;
	throw syntax_error(line_num);
}