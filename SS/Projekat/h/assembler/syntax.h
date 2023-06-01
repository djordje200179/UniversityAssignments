#pragma once

#include <stdlib.h>

struct symbol_list {
	char** arr;
	size_t size;
};

void symbol_append(struct symbol_list* symbols_list, char* symbol);

struct word_arg {
	enum { WORD_ARG_SYMBOL, WORD_ARG_LITERAL } type;

	union {
		char* symbol;
		size_t literal;
	};
};

struct word_args {
	struct word_arg* arr;
	size_t size;
};

void word_arg_append(struct word_args* word_args, struct word_arg word_arg);

struct dir {
	int type;

	union {
		struct symbol_list symbols;
		struct word_args word_args;

		char* name;
		size_t size;
		char* literal;
	};
};

struct operand {
	enum {
		OPERAND_LITERAL_VALUE,
		OPERAND_SYMBOL_VALUE,
		OPERAND_LITERAL_ADDR,
		OPERAND_SYMBOL_ADDR,
		OPERAND_REG_VALUE,
		OPERAND_REG_ADDR,
		OPERAND_REG_ADDR_WITH_LITERAL_OFFSET,
		OPERAND_REG_ADDR_WITH_SYMBOL_OFFSET
	} type;

	union {
		size_t literal;
		char* symbol;
		int reg;
	};
};

struct inst {
	int type;

	struct {
		int reg1;
		int reg2;
		struct operand operand;
	};
};

struct line {
	enum { LINE_EMPTY, LINE_DIR, LINE_INST } type;

	char* label;

	union {
		struct dir dir;
		struct inst inst;
	};
};

void line_print(struct line line);

struct line_list {
	struct line* arr;
	size_t size;
};

void line_append(struct line_list* lines, struct line line);