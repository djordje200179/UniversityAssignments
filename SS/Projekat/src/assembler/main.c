#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "assembler/syntax.h"

extern FILE* yyin;
extern int yyparse();

struct args {
	char* input_file_path;
	char* output_file_path;
};

struct args read_args(int argc, char** argv) {
	struct args args;
	args.output_file_path = "a.out";

	opterr = 0;
	char param;
	while ((param = getopt(argc, argv, "o:")) != -1) {
		switch (param) {
			case 'o':
				args.output_file_path = optarg;
				break;
			case '?':
				if (optopt == 'o')
					fprintf(stderr, "Option -o requires an argument.\n");
				else
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);

				exit(1);
			default:
				abort();
		}
	}

	int leftover_args_count = argc - optind;
	if (leftover_args_count != 1) {
		fprintf(stderr, "Expected 1 argument, got %d\n", leftover_args_count);
		exit(1);
	}
	args.input_file_path = argv[optind];

	return args;
}

struct lines read_input_file(char* input_file_path) {
	FILE* input_file = fopen(input_file_path, "r");
	if (!input_file) {
		fprintf(stderr, "Can't open file: %s\n", input_file_path);
		exit(2);
	}

	yyin = input_file;

	struct lines lines = {NULL, 0};
	yyparse(&lines);

	fclose(input_file);

	return lines;
}

void assemble(struct lines lines);

int main(int argc, char** argv) {
	struct args args = read_args(argc, argv);
	struct lines lines = read_input_file(args.input_file_path);

	// lines_print(lines);

	assemble(lines);

	return 0;
}

void yyerror(struct lines* ret_lines, const char* message) {
	extern int line_num;

	fprintf(stderr, "Syntax error at line %d.\n", line_num);
	exit(3);
}