#include "assembler/syntax.h"
#include <stdio.h>
#include <stdlib.h>
#include "assembler/parser.h"

void lines_append(struct lines* lines, struct line line) {
	lines->size++;
	lines->arr = realloc(lines->arr, lines->size * sizeof(struct line));

	lines->arr[lines->size - 1] = line;
}

static void print_inst_type(int inst_type) {
	switch (inst_type) {
		case INST_HALT:
			printf("HALT");
			break;
		case INST_INT:
			printf("INT");
			break;
		case INST_IRET:
			printf("IRET");
			break;
		case INST_RET:
			printf("RET");
			break;
		case INST_CALL:
			printf("CALL");
			break;
		case INST_JMP:
			printf("JMP");
			break;
		case INST_BEQ:
			printf("BEQ");
			break;
		case INST_ADD:
			printf("ADD");
			break;
		case INST_SUB:
			printf("SUB");
			break;
		case INST_MUL:
			printf("MUL");
			break;
		case INST_DIV:
			printf("DIV");
			break;
		case INST_AND:
			printf("AND");
			break;
		case INST_OR:
			printf("OR");
			break;
		case INST_XOR:
			printf("XOR");
			break;
		case INST_SHL:
			printf("SHL");
			break;
		case INST_SHR:
			printf("SHR");
			break;
		case INST_XCHG:
			printf("XCHG");
			break;
		case INST_PUSH:
			printf("PUSH");
			break;
		case INST_POP:
			printf("POP");
			break;
		case INST_NOT:
			printf("NOT");
			break;
		case INST_LD:
			printf("LD");
			break;
		case INST_ST:
			printf("ST");
			break;
		case INST_CSRRD:
			printf("CSRRD");
			break;
		case INST_CSRWR:
			printf("CSRWR");
			break;
		default:
			printf("INST UNKNOWN");
			break;
	}
}

static void print_operand(struct operand operand) {
	switch (operand.type) {
		case OPERAND_LITERAL_VALUE:
			printf("0x%lx", operand.int_literal);
			break;
		case OPERAND_SYMBOL_VALUE:
			printf("%s", operand.symbol);
			break;
		case OPERAND_LITERAL_ADDR:
			printf("[0x%lx]", operand.int_literal);
			break;
		case OPERAND_SYMBOL_ADDR:
			printf("[%s]", operand.symbol);
			break;
		case OPERAND_REG_VALUE:
			printf("%%r%d", operand.reg);
			break;
		case OPERAND_REG_ADDR:
			printf("[%%r%d]", operand.reg);
			break;
		default:
			printf("OPERAND UNKNOWN");
			break;
	}
}

static void print_csr(int reg) {
	putchar('%');
	switch (reg) {
		case 0:
			printf("status");
			break;
		case 1:
			printf("handler");
			break;
		case 2:
			printf("cause");
			break;
		default:
			printf("CSR UNKNOWN");
			break;
	}
}

static void print_inst(struct inst inst) {
	print_inst_type(inst.type);
	putchar(' ');

	switch (inst.type) {
		case INST_CALL:
		case INST_JMP:
			printf("(to ");
			print_operand(inst.params.operand);
			printf(")");
			break;
		case INST_BEQ:
		case INST_BNE:
		case INST_BGT:
			printf("(%%r%d <=> %%r%d, to ", inst.params.reg1, inst.params.reg2);
			print_operand(inst.params.operand);
			printf(")");
			break;
		case INST_ADD:
		case INST_SUB:
		case INST_MUL:
		case INST_DIV:
		case INST_AND:
		case INST_OR:
		case INST_XOR:
		case INST_SHL:
		case INST_SHR:
		case INST_XCHG:
			printf("(%%r%d, %%r%d)", inst.params.reg1, inst.params.reg2);
			break;
		case INST_PUSH:
		case INST_POP:
		case INST_NOT:
			printf("(%%r%d)", inst.params.reg1);
			break;
		case INST_LD:
			printf("(%%r%d <= ", inst.params.reg1);
			print_operand(inst.params.operand);
			printf(")");
			break;
		case INST_ST:
			printf("(");
			print_operand(inst.params.operand);
			printf(" <= %%r%d)", inst.params.reg1);
			break;
		case INST_CSRRD:
			printf("(%%r%d <= ", inst.params.reg2);
			print_csr(inst.params.reg1);
			printf(")");
			break;
		case INST_CSRWR:
			printf("(");
			print_csr(inst.params.reg2);
			printf(" <= %%r%d)", inst.params.reg1);
			break;
	}
}

static void print_dir(struct dir dir) {
	switch (dir.type) {
		case DIR_GLOBAL:
			printf("GLOBAL (");
			for (size_t i = 0; i < dir.operands.size; i++) {
				if (i != 0)
					printf(", ");
				printf("%s", dir.operands.arr[i].symbol);
			}
			printf(")");

			break;
		case DIR_EXTERN:
			printf("EXTERN (");
			for (size_t i = 0; i < dir.operands.size; i++) {
				if (i != 0)
					printf(", ");
				printf("%s", dir.operands.arr[i].symbol);
			}
			printf(")");

			break;
		case DIR_SECTION:
			printf("SECTION (%s)", dir.operand.symbol);
			break;
		case DIR_WORD:
			printf("WORD (");
			for (size_t i = 0; i < dir.operands.size; i++) {
				if (i != 0)
					printf(", ");

				switch (dir.operands.arr[i].type) {
					case CONST_OPERAND_SYMBOL:
						printf("%s", dir.operands.arr[i].symbol);
						break;
					case CONST_OPERAND_LITERAL:
						printf("%zu", dir.operands.arr[i].literal);
						break;
				}
			}
			printf(")");
			break;
		case DIR_SKIP:
			printf("SKIP (%zu bytes)", dir.size);
			break;
		case DIR_ASCII:
			printf("ASCII (\"%s\")", dir.str_literal);
			break;
		default:
			printf("DIR UNKNOWN");
			break;
	}
}

void line_print(struct line line) {
	switch (line.type) {
		case LINE_EMPTY:
			printf("EMPTY");
			break;
		case LINE_DIR:
			printf("DIR:\t");
			print_dir(line.dir);
			break;
		case LINE_INST:
			printf("INST:\t");
			print_inst(line.inst);
			break;
	}

	if (line.label)
		printf(" (%s)", line.label);

	putchar('\n');
}

void lines_print(struct lines lines) {
	for (size_t i = 0; i < lines.size; i++)
		line_print(lines.arr[i]);
}

void const_operands_append(struct const_operands* const_operands,
						   struct const_operand const_operand) {
	const_operands->size++;
	const_operands->arr =
		realloc(const_operands->arr,
				const_operands->size * sizeof(struct const_operand));

	const_operands->arr[const_operands->size - 1] = const_operand;
}

struct operand const_operand_to_operand(struct const_operand const_operand) {
	struct operand operand;

	switch (const_operand.type) {
		case CONST_OPERAND_LITERAL:
			operand.type = OPERAND_LITERAL_ADDR;
			operand.int_literal = const_operand.literal;
			break;
		case CONST_OPERAND_SYMBOL:
			operand.type = OPERAND_SYMBOL_ADDR;
			operand.symbol = const_operand.symbol;
			break;
	}

	return operand;
}