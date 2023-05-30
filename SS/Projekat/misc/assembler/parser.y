%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "../../h/assembler/syntax.h"
	
	extern int yylex();
	extern int yyparse();
	extern FILE *yyin;
	extern struct lines lines;

	extern int line_num;
	
	void yyerror(const char *s);
%}

%union {
	int ival;	
	float fval;
	char *sval;
}

%token ENDL

%token DIR_GLOBAL DIR_EXTERN DIR_SECTION DIR_WORD
%token DIR_SKIP DIR_ASCII DIR_EQU

%token INST_HALT INST_INT INST_IRET INST_CALL INST_RET
%token INST_JMP INST_BEQ INST_BNE INST_BGT
%token INST_PUSH INST_POP
%token INST_ADD INST_SUB INST_MUL INST_DIV
%token INST_NOT INST_AND INST_OR INST_XOR
%token INST_SHL INST_SHR
%token INST_LD INST_ST INST_XCHG INST_CSRRD INST_CSRWR

%token <ival> REG
%token <ival> CREG

%token <ival> INT_LITERAL
%token <sval> STR_LITERAL
%token <sval> SYMBOL

%%
assembly_file:
	directive_list {
		printf("Done with assembly file\n");
	}

directive:
	global_def | extern_def | section_def | skip_def | string_def | label_def;

directive_list:
	directive_list directive | directive;

CSL:
	CSL ',' SYMBOL | SYMBOL;

global_def:
	DIR_GLOBAL CSL ENDLS {
		struct line line = {
			.type = LINE_DIR,
			.label = NULL,
			.dir = DIR_GLOBAL
		};

		lines_append(&lines, line);
	};
extern_def:
	DIR_EXTERN CSL ENDLS {
		printf("Extern definition\n");
	};

section_def:
	DIR_SECTION SYMBOL ENDLS {
		printf("Section %s definition\n", $2);
	};

skip_def:
	DIR_SKIP INT_LITERAL ENDLS {
		printf("Skip of %d bytes definition\n", $2);
	};

string_def:
	DIR_ASCII STR_LITERAL ENDLS {
		printf("String \"%s\" definition\n", $2);
	};

label_def:
	SYMBOL ':' ENDLS | SYMBOL ':' {
		printf("Label %s definition\n", $1);
	};
	
ENDLS:
	ENDLS ENDL | ENDL;
%%

void yyerror(const char* message) {
	printf("ERROR!\n");
	printf("On line %d\n", line_num);
	printf("Message: %s\n", message);

	exit(-1);
}