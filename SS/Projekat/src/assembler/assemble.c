#include <stdio.h>
#include <string.h>
#include "assembler/parser.h"
#include "assembler/syntax.h"
#include "common/relocation.h"
#include "common/section.h"
#include "common/symbol.h"

static struct symbol_table first_phase(struct lines lines) {
	struct symbol_table symbol_table = {NULL, 0};

	size_t address = 0;
	size_t section_index = 0;
	for (size_t i = 0; i < lines.size; i++) {
		struct line line = lines.arr[i];
		// line_print(line);

		if (line.label) {
			struct symbol symbol = {.type = SYMBOL_TYPE_NOTYPE,
									.name = line.label,
									.value = address,
									.size = 0,
									.section = section_index,
									.global = false};

			symbol_table_append(&symbol_table, symbol);
		}

		switch (line.type) {
			case LINE_DIR:
				switch (line.dir.type) {
					case DIR_EXTERN:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							struct const_operand operand =
								line.dir.operands.arr[j];

							struct symbol symbol = {.type = SYMBOL_TYPE_NOTYPE,
													.name = operand.symbol,
													.value = 0,
													.size = 0,
													.section = SECTION_UNDEF,
													.global = true};

							symbol_table_append(&symbol_table, symbol);

							address += 4;
						}

						break;
					case DIR_SECTION: {
						symbol_table.arr[section_index].size = address;

						section_index = symbol_table.size;
						address = 0;

						struct symbol symbol = {.type = SYMBOL_TYPE_SECTION,
												.name = line.dir.operand.symbol,
												.value = 0,
												.size = 0,
												.section = section_index,
												.global = false};

						symbol_table_append(&symbol_table, symbol);

						break;
					}
					case DIR_WORD:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							struct const_operand operand =
								line.dir.operands.arr[j];

							if (operand.type == CONST_OPERAND_LITERAL) {
								address += 4;
								continue;
							}

							address += 4;
						}

						break;
					case DIR_SKIP:
						address += line.dir.size;
						break;
					case DIR_ASCII:
						address += strlen(line.dir.str_literal) + 1;
						break;
					case DIR_EQU:
						// FIXME
						break;
				}
				break;
			case LINE_INST:
				/* code */

				address += 4;

				break;
		}
	}

	symbol_table.arr[section_index].size = address;

	return symbol_table;
}

static struct sections create_sections(struct symbol_table symbol_table) {
	struct sections sections = {NULL, 0};
	for (int i = 0; i < symbol_table.size; i++) {
		const struct symbol symbol = symbol_table.arr[i];

		if (symbol.type != SYMBOL_TYPE_SECTION)
			continue;

		sections_append(&sections, section_init(symbol));
	}

	return sections;
};

static void second_phase(struct lines lines,
						 struct symbol_table symbol_table,
						 struct sections sections) {
	struct section* section = NULL;
	size_t address = 0;
	for (int i = 0; i < lines.size; i++) {
		struct line line = lines.arr[i];

		switch (line.type) {
			case LINE_DIR:
				switch (line.dir.type) {
					case DIR_GLOBAL:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							char* symbol_name = line.dir.operands.arr[j].symbol;
							struct symbol* symbol =
								symbol_table_find(symbol_table, symbol_name);

							symbol->global = true;
						}

						break;
					case DIR_SECTION:
						section = section ? section + 1 : sections.arr;
						address = 0;
						break;
					case DIR_WORD:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							struct const_operand operand =
								line.dir.operands.arr[j];

							switch (operand.type) {
								case CONST_OPERAND_LITERAL:
									section_write(section, &operand.literal, 4);
									break;
								case CONST_OPERAND_SYMBOL: {
									struct symbol* symbol = symbol_table_find(
										symbol_table, operand.symbol);

									struct relocation relocation = {
										.offset = address};

									if (!symbol) {
										fprintf(stderr,
												"Symbol %s not found.\n",
												operand.symbol);
										exit(4);
									}
									if (symbol->global) {
										relocation.symbol =
											symbol - symbol_table.arr;
										relocation.addend = 0;
									} else {
										relocation.symbol = symbol->section;
										relocation.addend = symbol->value;
									}

									relocation_table_append(
										&section->relocation_table, relocation);
									break;
								}
							}
							address += 4;
						}
						break;
					case DIR_SKIP:
						address += line.dir.size;
						break;
					case DIR_ASCII: {
						size_t literal_size = strlen(line.dir.str_literal) + 1;
						section_write(section, line.dir.str_literal,
									  literal_size);
						address += literal_size;

						break;
					}
					case DIR_EQU:
						// FIXME
						break;
				}
				break;
			case LINE_INST:
				/* code */

				address += 4;

				break;
		}
	}
}

void assemble(struct lines lines) {
	struct symbol_table symbol_table = first_phase(lines);
	struct sections sections = create_sections(symbol_table);
	second_phase(lines, symbol_table, sections);

	symbol_table_print(symbol_table);
	sections_print(sections);
}