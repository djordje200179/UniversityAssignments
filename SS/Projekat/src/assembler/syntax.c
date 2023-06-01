#include "../../h/assembler/syntax.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../h/assembler/parser.h"

void line_print(struct line line) {
	switch (line.type) {
		case LINE_EMPTY:
			printf("LINE_EMPTY\n");
			break;
		case LINE_DIR:
			printf("LINE_DIR:\t");

			switch (line.dir.type) {
				case DIR_GLOBAL:
					printf("GLOBAL (");
					for (size_t i = 0; i < line.dir.symbols.size; i++) {
						if (i != 0)
							printf(", ");
						printf("%s", line.dir.symbols.arr[i]);
					}
					printf(")\n");

					break;
				case DIR_EXTERN:
					printf("EXTERN (");
					for (size_t i = 0; i < line.dir.symbols.size; i++) {
						if (i != 0)
							printf(", ");
						printf("%s", line.dir.symbols.arr[i]);
					}
					printf(")\n");

					break;
				case DIR_SECTION:
					printf("SECTION (%s)\n", line.dir.name);
					break;
				case DIR_WORD:
					printf("WORD (");
					for (size_t i = 0; i < line.dir.word_args.size; i++) {
						if (i != 0)
							printf(", ");

						switch (line.dir.word_args.arr[i].type) {
							case WORD_ARG_SYMBOL:
								printf("SYM: %s",
									   line.dir.word_args.arr[i].symbol);
								break;
							case WORD_ARG_LITERAL:
								printf("L: %zu",
									   line.dir.word_args.arr[i].literal);
								break;
						}
					}
					printf(")\n");
					break;
				case DIR_SKIP:
					printf("SKIP (%zu bytes)\n", line.dir.size);
					break;
				case DIR_ASCII:
					printf("ASCII (\"%s\")\n", line.dir.literal);
					break;
				default:
					printf("DIR_UNKNOWN\n");
					break;
			}

			break;
		case LINE_INST:
			printf("LINE_INST:\t");

			switch (line.inst.type) {
				case INST_HALT:
					printf("HALT\n");
					break;
				case INST_INT:
					printf("INT\n");
					break;
				case INST_IRET:
					printf("IRET\n");
					break;
				case INST_CALL:
					printf("CALL (to ");
					switch (line.inst.operand.type) {
						case OPERAND_LITERAL_ADDR:
							printf("L: 0x%lx", line.inst.operand.literal);
							break;
						case OPERAND_SYMBOL_ADDR:
							printf("SYM: %s", line.inst.operand.symbol);
							break;
					}
					printf(")\n");
					break;
				case INST_RET:
					printf("RET\n");
					break;
				case INST_JMP:
					printf("JMP (to ");
					switch (line.inst.operand.type) {
						case OPERAND_LITERAL_ADDR:
							printf("L: 0x%lx", line.inst.operand.literal);
							break;
						case OPERAND_SYMBOL_ADDR:
							printf("SYM: %s", line.inst.operand.symbol);
							break;
					}
					printf(")\n");
					break;
				default:
					printf("INST_UNKNOWN\n");
					break;
			}

			break;
	}
}

void symbol_append(struct symbol_list* symbols, char* symbol) {
	symbols->size++;
	symbols->arr = realloc(symbols->arr, symbols->size * sizeof(char*));

	symbols->arr[symbols->size - 1] = symbol;
}

void word_arg_append(struct word_args* word_args, struct word_arg word_arg) {
	word_args->size++;
	word_args->arr =
		realloc(word_args->arr, word_args->size * sizeof(struct word_arg));

	word_args->arr[word_args->size - 1] = word_arg;
}

void line_append(struct line_list* lines, struct line line) {
	lines->size++;
	lines->arr = realloc(lines->arr, lines->size * sizeof(struct line));

	lines->arr[lines->size - 1] = line;
}