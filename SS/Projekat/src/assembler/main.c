#include <stdio.h>
#include <stdlib.h>
#include "../../h/assembler/syntax.h"

struct lines lines = {NULL, 0};

extern FILE* yyin;
extern int yylex();

int main(int argc, char** argv) {
	FILE* file = fopen(argv[1], "r");
	if (!file) {
		printf("Can't open file: %s\n", argv[1]);
		return -1;
	}

	yyin = file;

	yylex();
}