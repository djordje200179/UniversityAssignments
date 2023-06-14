extern "C" {
#include "assembler/parser.h"
#include "assembler/syntax.h"
}

#include <cstring>
#include <iostream>
#include "common/instruction.hpp"
#include "common/relocation.hpp"
#include "common/section.hpp"
#include "common/symbol.hpp"

symbol_table first_phase(const lines& lines) {
	symbol_table symbol_table;

	size_t address = 0;
	size_t section_index = 0;
	for (size_t i = 0; i < lines.size; i++) {
		const auto& line = lines.arr[i];
		// line_print(line);

		if (line.label) {
			symbol symbol = {.type = symbol::type::NOTYPE,
							 .name = line.label,
							 .value = address,
							 .section = section_index,
							 .global = false};

			symbol_table.push_back(symbol);
		}

		switch (line.type) {
			case line::LINE_DIR:
				switch (line.dir.type) {
					case DIR_EXTERN:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							const auto& operand = line.dir.operands.arr[j];

							symbol symbol = {.type = symbol::type::NOTYPE,
											 .name = operand.symbol,
											 .value = 0,
											 .section = SECTION_UNDEF,
											 .global = true};

							symbol_table.push_back(symbol);

							address += 4;
						}

						break;
					case DIR_SECTION: {
						section_index = symbol_table.size();
						address = 0;

						symbol symbol = {.type = symbol::type::SECTION,
										 .name = line.dir.operand.symbol,
										 .value = 0,
										 .section = section_index,
										 .global = false};

						symbol_table.push_back(symbol);

						break;
					}
					case DIR_WORD:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							const auto& operand = line.dir.operands.arr[j];

							if (operand.type ==
								const_operand::CONST_OPERAND_INT_LITERAL) {
								address += 4;
								continue;
							}

							address += 4;
						}

						break;
					case DIR_SKIP:
						address += line.dir.operand.int_literal;
						break;
					case DIR_ASCII:
						address += strlen(line.dir.operand.str_literal) + 1;
						break;
					case DIR_EQU:
						// FIXME
						break;
				}
				break;
			case line::LINE_INST:
				/* code */

				address += 4;

				break;
		}
	}

	return symbol_table;
}

static std::vector<section> create_sections(const symbol_table& symbol_table) {
	std::vector<section> sections;
	for (const auto& symbol : symbol_table) {
		if (symbol.type != symbol::type::SECTION)
			continue;

		sections.push_back(section(symbol));
	}

	return sections;
};

static void second_phase(const lines& lines,
						 symbol_table& symbol_table,
						 std::vector<section>& sections) {
	section* section = NULL;
	size_t address = 0;
	for (int i = 0; i < lines.size; i++) {
		const auto& line = lines.arr[i];

		switch (line.type) {
			case line::LINE_DIR:
				switch (line.dir.type) {
					case DIR_GLOBAL:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							auto symbol_name = line.dir.operands.arr[j].symbol;
							auto symbol = symbol_table.find(symbol_name);

							symbol->global = true;
						}

						break;
					case DIR_SECTION:
						section = section ? section + 1 : &sections[0];
						address = 0;
						break;
					case DIR_WORD:
						for (size_t j = 0; j < line.dir.operands.size; j++) {
							const auto& operand = line.dir.operands.arr[j];

							switch (operand.type) {
								case const_operand::CONST_OPERAND_INT_LITERAL:
									/*section_write(section,
									   &operand.int_literal, 4);*/
									break;
								case const_operand::CONST_OPERAND_SYMBOL: {
									auto symbol =
										symbol_table.find(operand.symbol);

									relocation relocation = {.offset = address};

									if (!symbol) {
										fprintf(stderr,
												"Symbol %s not found.\n",
												operand.symbol);
										exit(4);
									}
									if (symbol->global) {
										relocation.symbol =
											symbol - &symbol_table[0];
										relocation.addend = 0;
									} else {
										relocation.symbol = symbol->section;
										relocation.addend = symbol->value;
									}

									section->relocation_table.push_back(
										relocation);
									break;
								}
							}
							address += 4;
						}
						break;
					case DIR_SKIP:
						address += line.dir.operand.int_literal;
						break;
					case DIR_ASCII: {
						auto literal = line.dir.operand.str_literal;
						auto literal_size = strlen(literal) + 1;
						// section_write(section, literal, literal_size);
						address += literal_size;

						break;
					}
					case DIR_EQU:
						// FIXME
						break;
				}
				break;
			case line::LINE_INST: {
				instruction instr;
				switch (line.inst.type) {
					case INST_BEQ:
					case INST_BNE:
					case INST_BGT:
					case INST_CALL:
					case INST_JMP:
					case INST_HALT:
						instr = instruction::make_halt();
						break;
					case INST_INT:
						instr = instruction::make_int();
						break;
					case INST_IRET:
					case INST_RET:
					case INST_PUSH:
					case INST_POP:
					case INST_NOT:
					case INST_XCHG:
					case INST_ADD:
					case INST_SUB:
					case INST_MUL:
					case INST_DIV:
					case INST_AND:
					case INST_OR:
					case INST_XOR:
					case INST_SHL:
					case INST_SHR:
					case INST_LD:
					case INST_ST:
					case INST_CSRRD:
					case INST_CSRWR:
						break;
				}

				// section_write(section, &instruction, 4);

				switch (line.inst.type) {
					case INST_BEQ:
					case INST_BNE:
					case INST_BGT:
					case INST_CALL:
					case INST_JMP:
					case INST_HALT:
					case INST_INT:
					case INST_IRET:
					case INST_RET:
					case INST_PUSH:
					case INST_POP:
					case INST_NOT:
					case INST_XCHG:
					case INST_ADD:
					case INST_SUB:
					case INST_MUL:
					case INST_DIV:
					case INST_AND:
					case INST_OR:
					case INST_XOR:
					case INST_SHL:
					case INST_SHR:
					case INST_LD:
					case INST_ST:
					case INST_CSRRD:
					case INST_CSRWR:
						break;
				}

				address += 4;

				break;
			}
		}
	}
}

void assemble(lines lines) {
	auto symbol_table = first_phase(lines);
	auto sections = create_sections(symbol_table);
	second_phase(lines, symbol_table, sections);

	std::cout << symbol_table;
	for (int i = 0; i < sections.size(); i++)
		std::cout << sections[i] << std::endl;
}