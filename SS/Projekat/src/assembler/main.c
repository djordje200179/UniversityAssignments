#include <stdio.h>
#include <stdlib.h>
#include "../../h/assembler/syntax.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	FILE* file = fopen(argv[1], "r");
	if (!file) {
		printf("Can't open file: %s\n", argv[1]);
		return -1;
	}

	yyin = file;

	struct line_list lines = {NULL, 0};
	yyparse(&lines);

	for (size_t i = 0; i < lines.size; i++) {
		line_print(lines.arr[i]);
	}

	return 0;
}