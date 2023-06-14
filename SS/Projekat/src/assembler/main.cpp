extern "C" {
#include <stdio.h>
#include "assembler/args.h"
#include "assembler/syntax.h"
}

#include <string>
#include "common/instruction.hpp"

extern "C" FILE* yyin;
extern "C" int yyparse(lines* ret_lines);

lines read_input_file(const std::string& input_file_path) {
	FILE* input_file = fopen(input_file_path.c_str(), "r");
	if (!input_file) {
		fprintf(stderr, "Can't open file: %s\n", input_file_path.c_str());
		exit(2);
	}

	yyin = input_file;

	struct lines lines = {NULL, 0};
	yyparse(&lines);

	fclose(input_file);

	return lines;
}

void assemble(lines lines);

int main(int argc, char** argv) {
	auto args = read_args(argc, argv);
	auto lines = read_input_file(args.input_file_path);

	// lines_print(lines);

	assemble(lines);

	return 0;
}

extern "C" void yyerror(lines* ret_lines, const char* message) {
	extern int line_num;

	fprintf(stderr, "Syntax error at line %d.\n", line_num);
	exit(3);
}