#pragma once

#include <stdlib.h>

struct const_operand {
	enum { CONST_OPERAND_SYMBOL, CONST_OPERAND_LITERAL } type;

	union {
		char* symbol;
		size_t literal;
	};
};

struct const_operands {
	struct const_operand* arr;
	size_t size;
};

void const_operands_append(struct const_operands* const_operands,
						   struct const_operand const_operand);

struct dir {
	int type;

	union {
		struct const_operand operand;
		struct const_operands operands;

		size_t size;
		char* str_literal;
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
		size_t int_literal;
		char* symbol;

		struct {
			int reg;
			struct const_operand offset;
		};
	};
};

struct operand const_operand_to_operand(struct const_operand const_operand);

struct inst_params {
	int reg1;
	int reg2;
	struct operand operand;
};

struct inst {
	int type;

	struct inst_params params;
};

struct line {
	enum { LINE_EMPTY, LINE_DIR, LINE_INST } type;

	char* label;

	union {
		struct dir dir;
		struct inst inst;
	};
};

struct lines {
	struct line* arr;
	size_t size;
};

void lines_append(struct lines* lines, struct line line);
void line_print(struct line line);
void lines_print(struct lines lines);