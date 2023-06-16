extern "C" {
#include "assembler/parser.h"
#include "assembler/syntax.h"
}

#include <cstring>
#include <iostream>
#include "assembler/exceptions.hpp"
#include "common/instruction.hpp"
#include "common/relocation.hpp"
#include "common/section.hpp"
#include "common/symbol.hpp"
#include "common/object_file.hpp"

symbol_table first_phase(lines lines) {
	symbol_table symbol_table;

	size_t address = 0;
	size_t section_index = 0;
	for (size_t i = 0; i < lines.size; i++) {
		const auto& line = lines.arr[i];

		if (line.label)
			symbol_table.push_back(symbol{
				.type = symbol::type::NOTYPE,
				.name = line.label,
				.value = address,
				.section = section_index,
				.global = false
			});

		switch (line.type) {
		case line::LINE_DIR:
			switch (line.dir.type) {
			case DIR_EXTERN:
				for (size_t j = 0; j < line.dir.operands.size; j++) {
					const auto& operand = line.dir.operands.arr[j];

					symbol_table.push_back(symbol{
						.type = symbol::type::NOTYPE,
						.name = operand.symbol,
						.value = 0,
						.section = SECTION_UNDEF,
						.global = true
					});

					address += 4;
				}

				break;
			case DIR_SECTION: {
				section_index = symbol_table.size();
				address = 0;

				symbol_table.push_back(symbol {
					.type = symbol::type::SECTION,
					.name = line.dir.operand.symbol,
					.value = 0,
					.section = section_index,
					.global = false
				});

				break;
			}
			case DIR_WORD:
				address += line.dir.operands.size * 4;
				break;
			case DIR_SKIP:
				address += line.dir.operand.int_literal;
				break;
			case DIR_ASCII:
				address += strlen(line.dir.operand.str_literal) + 1;
				break;
			case DIR_EQU:
				// TODO: Process 'equ' directive
				break;
			}
			break;
		case line::LINE_INST:
			switch(line.inst.type) {
			case INST_UNCOND_JUMP:
			case INST_COND_JUMP:
				address += 8;
				break;
			case INST_LD:
			case INST_ST:
				switch(line.inst.params.operand.type) {
				case operand::OPERAND_LITERAL_VALUE:
				case operand::OPERAND_SYMBOL_VALUE:
				case operand::OPERAND_LITERAL_ADDR:
				case operand::OPERAND_SYMBOL_ADDR:
					address += 8;
					break;
				}
				break;
			}

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

static void second_phase(lines lines,
						 symbol_table& symbol_table,
						 std::vector<section>& sections) {
	section* section = NULL;
	for (size_t i = 0; i < lines.size; i++) {
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
				break;
			case DIR_WORD:
				for (size_t j = 0; j < line.dir.operands.size; j++) {
					const auto& operand = line.dir.operands.arr[j];

					switch (operand.type) {
						case const_operand::CONST_OPERAND_INT_LITERAL:
							section->append(&operand.int_literal, sizeof(int));
							break;
						case const_operand::CONST_OPERAND_SYMBOL: {
							auto symbol = symbol_table.find(operand.symbol);
							if (!symbol)
								throw symbol_not_found_error(operand.symbol);

							relocation relocation = {
								.offset = section->size()
							};

							if (symbol->global) {
								relocation.symbol = symbol - &symbol_table[0];
								relocation.addend = 0;
							} else {
								relocation.symbol = symbol->section;
								relocation.addend = symbol->value;
							}

							section->relocation_table.push_back(relocation);

							section->append(nullptr, sizeof(int));

							break;
						}
					}
				}
				break;
			case DIR_SKIP:
				section->append(nullptr, line.dir.operand.int_literal);
				break;
			case DIR_ASCII: {
				auto literal = line.dir.operand.str_literal;
				section->append(literal, strlen(literal) + 1);

				break;
			}
			case DIR_EQU:
				// TODO: Implement 'equ' directive
				break;
			}
			break;
		case line::LINE_INST: {
			switch (line.inst.type) {
			case INST_BEQ:
				// TODO: Implement 'beq' instruction
				break;
			case INST_BNE:
				// TODO: Implement 'bne' instruction
				break;
			case INST_BGT:
				// TODO: Implement 'bgt' instruction
				break;
			case INST_CALL:
				// TODO: Implement 'call' instruction
				break;
			case INST_JMP:
				// TODO: Implement 'jmp' instruction
				break;
			case INST_HALT: {
				auto inst = instruction::make_halt();
				section->append(&inst, 4);
				break;
			}	
			case INST_INT: {
				auto inst = instruction::make_int();
				section->append(&inst, 4);
				break;
			}
			case INST_IRET:
				// TODO: Implement 'iret' instruction
				break;
			case INST_RET:
				// TODO: Implement 'ret' instruction
				break;
			case INST_PUSH:
				// TODO: Implement 'push' instruction
				break;
			case INST_POP:
				// TODO: Implement 'pop' instruction
				break;
			case INST_NOT: {
				auto inst = instruction::make_logical(
					instruction::logical_operation::NOT,
					line.inst.params.reg1,
					line.inst.params.reg1,
					0
				);
				section->append(&inst, 4);
				break;
			}
			case INST_XCHG: {
				auto inst = instruction::make_xchg(
					line.inst.params.reg1,
					line.inst.params.reg2
				);
				section->append(&inst, 4);
				break;
			}
			case INST_ADD: {
				auto inst = instruction::make_arithmetic(
					instruction::arithmetic_operation::ADD,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_SUB: {
				auto inst = instruction::make_arithmetic(
					instruction::arithmetic_operation::SUB,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_MUL: {
				auto inst = instruction::make_arithmetic(
					instruction::arithmetic_operation::MUL,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_DIV: {
				auto inst = instruction::make_arithmetic(
					instruction::arithmetic_operation::DIV,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_AND: {
				auto inst = instruction::make_logical(
					instruction::logical_operation::AND,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_OR: {
				auto inst = instruction::make_logical(
					instruction::logical_operation::OR,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_XOR: {
				auto inst = instruction::make_logical(
					instruction::logical_operation::XOR,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_SHL: {
				auto inst = instruction::make_shift(
					false,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_SHR: {
				auto inst = instruction::make_shift(
					true,
					line.inst.params.reg2,
					line.inst.params.reg2,
					line.inst.params.reg1
				);
				section->append(&inst, 4);
				break;
			}
			case INST_LD:
				// TODO: Implement 'ld' instruction
				break;
			case INST_ST:
				// TODO: Implement 'st' instruction
				break;
			case INST_CSRRD:
				// TODO: Implement 'csrrd' instruction
				break;
			case INST_CSRWR:
				// TODO: Implement 'csrwr' instruction
				break;
			}

			break;
		}
		}
	}
}

object_file assemble(lines lines) {
	auto symbol_table = first_phase(lines);
	auto sections = create_sections(symbol_table);
	second_phase(lines, symbol_table, sections);

	return object_file{
		.symbols = symbol_table,
		.sections = sections
	};
}