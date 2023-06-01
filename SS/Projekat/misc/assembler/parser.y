%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "../../h/assembler/syntax.h"

	extern int yylex();
	extern FILE *yyin;

	extern int line_num;
	
	void yyerror(struct line_list* ret_lines, const char *s);
%}

%code requires {
	#include "syntax.h"
}

%union {
	int ival;	
	float fval;
	char* sval;

	struct symbol_list symbols;
	struct word_args word_args;
	struct operand operand;

	struct dir dir;
	struct inst inst;
	struct line line;
	struct line_list lines;
}

%token ENDL

%token DIR_GLOBAL DIR_EXTERN DIR_SECTION
%token DIR_WORD DIR_SKIP DIR_ASCII DIR_EQU

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

%type <symbols> symbol_list

%type <symbols> global
%type <symbols> extrn
%type <sval> section
%type <word_args> word_args
%type <word_args> word
%type <ival> skip
%type <sval> ascii

%type <operand> call
%type <operand> jmp

%type <dir> directive
%type <inst> instruction
%type <line> line

%type <lines> lines

%parse-param {struct line_list* ret_lines}

%%
assembly_file:
	lines {
		printf("Done with assembly file\n");
		*ret_lines = $1;
	}

lines:
	lines line {
		line_append(&$$, $2);
	} | line {
		$$ = (struct line_list){NULL, 0};

		line_append(&$$, $1);
	};

line:
	directive ENDLS {
		$$.type = LINE_DIR;
		$$.dir = $1;
	} | instruction ENDLS {
		$$.type = LINE_INST;
		$$.inst = $1;
	};

ENDLS:
	ENDLS ENDL | ENDL;

directive:
	global {
		$$.type = DIR_GLOBAL;
		$$.symbols = $1;
	} | extrn {
		$$.type = DIR_EXTERN;
		$$.symbols = $1;
	} | section {
		$$.type = DIR_SECTION;
		$$.name = $1;
	} | word {
		$$.type = DIR_WORD;
		$$.word_args = $1;
	} | skip {
		$$.type = DIR_SKIP;
		$$.size = $1;
	} | ascii {
		$$.type = DIR_ASCII;
		$$.literal = $1;
	};

symbol_list:
	symbol_list ',' SYMBOL {
		symbol_append(&$$, $3);
	} | SYMBOL {
		$$ = (struct symbol_list){NULL, 0};

		symbol_append(&$$, $1);
	}

global:
	DIR_GLOBAL symbol_list { $$ = $2; };

extrn:
	DIR_EXTERN symbol_list { $$ = $2; };

section:
	DIR_SECTION SYMBOL { $$ = $2; };

word_args:
	word_args ',' INT_LITERAL {
		struct word_arg arg;
		arg.type = WORD_ARG_LITERAL;
		arg.literal = $3;
		
		word_arg_append(&$$, arg);
	} | word_args ',' SYMBOL {
		struct word_arg arg;
		arg.type = WORD_ARG_SYMBOL;
		arg.symbol = $3;

		word_arg_append(&$$, arg);
	} | INT_LITERAL {
		$$ = (struct word_args){NULL, 0};

		struct word_arg arg;
		arg.type = WORD_ARG_LITERAL;
		arg.literal = $1;

		word_arg_append(&$$, arg);
	} | SYMBOL {
		$$ = (struct word_args){NULL, 0};

		struct word_arg arg;
		arg.type = WORD_ARG_SYMBOL;
		arg.symbol = $1;

		word_arg_append(&$$, arg);
	};
word:
	DIR_WORD word_args { $$ = $2; };

skip:
	DIR_SKIP INT_LITERAL { $$ = $2;};

ascii:
	DIR_ASCII STR_LITERAL { $$ = $2; };

// label_def:
// 	SYMBOL ':' ENDLS | SYMBOL ':' {
// 		printf("Label %s definition\n", $1);
// 	};

instruction:
	halt {
		$$.type = INST_HALT;
	} | interrupt {
		$$.type = INST_INT;
	} | iret {
		$$.type = INST_IRET;
	} | call {
		$$.type = INST_CALL;
		$$.operand = $1;
	} | ret {
		$$.type = INST_RET;
	} | jmp {
		$$.type = INST_JMP;
		$$.operand = $1;
	};

halt:
	INST_HALT { };

interrupt:
	INST_INT { };

iret:
	INST_IRET { };

call:
	INST_CALL SYMBOL {
		$$.type = OPERAND_SYMBOL_ADDR;
		$$.symbol = $2;
	} | INST_CALL INT_LITERAL {
		$$.type = OPERAND_LITERAL_ADDR;
		$$.literal = $2;
	};

ret:
	INST_RET { };

jmp:
	INST_JMP SYMBOL {
		$$.type = OPERAND_SYMBOL_ADDR;
		$$.symbol = $2;
	} | INST_JMP INT_LITERAL {
		$$.type = OPERAND_LITERAL_ADDR;
		$$.literal = $2;
	};
%%

void yyerror(struct line_list* ret_lines, const char* message) {
	printf("ERROR!\n");
	printf("On line %d\n", line_num);
	printf("Message: %s\n", message);

	exit(-1);
}