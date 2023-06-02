%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "assembler/syntax.h"

	extern int yylex();
	extern FILE *yyin;

	void yyerror(struct lines* ret_lines, const char* s);
	// int yydebug = 1;
%}

%code requires {
	#include "assembler/syntax.h"

	// #define YYDEBUG 1
}

%union {
	int int_literal;
	char* str_literal;

	char* symbol;

	int reg;
	int inst_type;

	struct const_operand const_operand;
	struct const_operands const_operands;
	struct operand operand;
	struct inst_params inst_params;

	struct dir dir;
	struct inst inst;
	struct line line;
	struct lines lines;
}

%token ENDL

%token DIR_GLOBAL DIR_EXTERN DIR_SECTION
%token DIR_WORD DIR_SKIP DIR_ASCII DIR_EQU

%token INST_BEQ INST_BNE INST_BGT
%token <inst_type> INST_COND_JUMP

%token INST_CALL INST_JMP
%token <inst_type> INST_UNCOND_JUMP

%token INST_HALT INST_INT INST_IRET INST_RET
%token <inst_type> INST_PARAMLESS

%token INST_PUSH INST_POP INST_NOT
%token <inst_type> INST_UNIPARAM

%token INST_XCHG INST_ADD INST_SUB INST_MUL INST_DIV
%token INST_AND INST_OR INST_XOR INST_SHL INST_SHR
%token <inst_type> INST_BIPARAM

%token INST_LD INST_ST INST_CSRRD INST_CSRWR

%token <reg> REG
%token <reg> CREG

%token <int_literal> INT_LITERAL
%token <str_literal> STR_LITERAL
%token <symbol> SYMBOL
%type <const_operand> const_operand
%type <const_operands> const_operands

%type <const_operands> global
%type <const_operands> extrn
%type <const_operand> section
%type <const_operands> word
%type <int_literal> skip
%type <str_literal> ascii

%type <operand> operand

%type <inst> unconditional_jump
%type <inst> conditional_jump
%type <inst> uniparam_inst
%type <inst> biparam_inst

%type <inst_params> ld
%type <inst_params> st
%type <inst_params> csrrd
%type <inst_params> csrwr

%type <symbol> label

%type <dir> directive
%type <inst> instruction
%type <line> line_content
%type <line> line

%type <lines> lines

%parse-param {struct lines* ret_lines}

%define parse.lac full
%define parse.error verbose

%%
assembly_file:
	lines {
		*ret_lines = $1;
	}

lines:
	lines line {
		lines_append(&$$, $2);
	} | line {
		$$ = (struct lines){NULL, 0};
		lines_append(&$$, $1);
	};

line:
	line_content ENDLS {
		$$ = $1;
		$$.label = NULL;
	} | label line_content ENDLS {
		$$ = $2;
		$$.label = $1;
	} | label ENDLS {
		$$.type = LINE_EMPTY; 
		$$.label = $1;
	}

line_content:
	directive {
		$$.type = LINE_DIR;
		$$.dir = $1;
	} | instruction {
		$$.type = LINE_INST;
		$$.inst = $1;
	};

label:
	SYMBOL ':' { $$ = $1; };

ENDLS:
	ENDLS ENDL | ENDL;

const_operand:
	INT_LITERAL {
		$$.type = CONST_OPERAND_LITERAL;
		$$.literal = $1;
	} | SYMBOL {
		$$.type = CONST_OPERAND_SYMBOL;
		$$.symbol = $1;
	};

const_operands:
	const_operands ',' const_operand {
		const_operands_append(&$$, $3);
	} | const_operand {
		$$ = (struct const_operands){NULL, 0};
		const_operands_append(&$$, $1);
	};
	
directive:
	global {
		$$.type = DIR_GLOBAL;
		$$.operands = $1;
	} | extrn {
		$$.type = DIR_EXTERN;
		$$.operands = $1;
	} | section {
		$$.type = DIR_SECTION;
		$$.operand = $1;
	} | word {
		$$.type = DIR_WORD;
		$$.operands = $1;
	} | skip {
		$$.type = DIR_SKIP;
		$$.size = $1;
	} | ascii {
		$$.type = DIR_ASCII;
		$$.str_literal = $1;
	};

global:
	DIR_GLOBAL const_operands { $$ = $2; };

extrn:
	DIR_EXTERN const_operands { $$ = $2; };

section:
	DIR_SECTION const_operand { $$ = $2; };

word:
	DIR_WORD const_operands { $$ = $2; };

skip:
	DIR_SKIP INT_LITERAL { $$ = $2;};

ascii:
	DIR_ASCII STR_LITERAL { $$ = $2; };

instruction:
	INST_PARAMLESS {
		$$.type = $1;
	} | unconditional_jump {
		$$ = $1;
	} | conditional_jump {
		$$ = $1;
	} | uniparam_inst {
		$$ = $1;
	} | biparam_inst {
		$$ = $1;
	} | ld {
		$$.type = INST_LD;
		$$.params = $1;
	} | st {
		$$.type = INST_ST;
		$$.params = $1;
	} | csrrd {
		$$.type = INST_CSRRD;
		$$.params = $1;
	} | csrwr {
		$$.type = INST_CSRWR;
		$$.params = $1;
	};

unconditional_jump:
	INST_UNCOND_JUMP const_operand {
		$$.type = $1;
		$$.params.operand = const_operand_to_operand($2);
	}

operand:
	'$' INT_LITERAL {
		$$.type = OPERAND_LITERAL_VALUE;
		$$.int_literal = $2;
	} | '$' SYMBOL {
		$$.type = OPERAND_SYMBOL_VALUE;
		$$.symbol = $2;
	} | INT_LITERAL {
		$$.type = OPERAND_LITERAL_ADDR;
		$$.int_literal = $1;
	} | SYMBOL {
		$$.type = OPERAND_SYMBOL_ADDR;
		$$.symbol = $1;
	} | REG {
		$$.type = OPERAND_REG_VALUE;
		$$.reg = $1;
	} | '[' REG ']' {
		$$.type = OPERAND_REG_ADDR;
		$$.reg = $2;
	} | '[' REG '+' INT_LITERAL ']' {
		$$.type = OPERAND_REG_ADDR_WITH_LITERAL_OFFSET;
		$$.reg = $2;
		$$.int_literal = $4;
	} | '[' REG '+' SYMBOL ']' {
		$$.type = OPERAND_REG_ADDR_WITH_SYMBOL_OFFSET;
		$$.reg = $2;
		$$.symbol = $4;
	};

conditional_jump:
	INST_COND_JUMP REG ',' REG ',' operand {
		$$.type = $1;
		$$.params.reg1 = $2;
		$$.params.reg2 = $4;
		$$.params.operand = $6;
	};

uniparam_inst:
	INST_UNIPARAM REG {
		$$.type = $1;
		$$.params.reg1 = $2;
	}

biparam_inst:
	INST_BIPARAM REG ',' REG {
		$$.type = $1;
		$$.params.reg1 = $2;
		$$.params.reg2 = $4;
	}

ld:
	INST_LD operand ',' REG {
		$$.operand = $2;
		$$.reg1 = $4;
	}

st:
	INST_ST REG ',' operand {
		$$.reg1 = $2;
		$$.operand = $4;
	}

csrrd:
	INST_CSRRD CREG ',' REG {
		$$.reg1 = $2;
		$$.reg2 = $4;
	}
	
csrwr:
	INST_CSRWR REG ',' CREG {
		$$.reg1 = $2;
		$$.reg2 = $4;
	}
%%