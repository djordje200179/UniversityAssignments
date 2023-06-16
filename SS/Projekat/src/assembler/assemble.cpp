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
			case INST_CALL:
				address += 2*4;
			case INST_UNCOND_JUMP:
			case INST_COND_JUMP:
				address += 8;
				address += 4;
				break;
			case INST_HALT:
			case INST_INT:
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
			case INST_CSRRD:
			case INST_CSRWR:
				address += 4;
				break;
			case INST_IRET:
				address += 4*4;
				break;
			case INST_RET:
			case INST_PUSH:
			case INST_POP:
				address += 2*4;
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

				address += 4;
				break;
			}

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
}

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
					if(!symbol)
						throw symbol_not_found_error(symbol_name);

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
			case INST_CALL: {
				auto inst_1 = instruction::make_arithmetic(
					instruction::arithmetic_operation::SUB,
					14,
					14,
					4
				);
				section->append(&inst_1, 4);

				auto inst_2 = instruction::make_store(
					instruction::store_mode::MEMDIR,
					14,
					0,
					15,
					0
				);
				section->append(&inst_2, 4);
			}
			case INST_BEQ:
			case INST_BNE:
			case INST_BGT:
			case INST_JMP: {
				instruction::jump_mode jump_mode;
				switch(line.inst.type) {
				case INST_BEQ:
					jump_mode = instruction::jump_mode::EQUAL;
					break;
				case INST_BNE:
					jump_mode = instruction::jump_mode::NOT_EQUAL;
					break;
				case INST_BGT:
					jump_mode = instruction::jump_mode::GREATER;
					break;
				default:
					jump_mode = instruction::jump_mode::UNCONDITIONAL;
					break;
				}

				switch (line.inst.params.operand.type) {
				// TODO: Implement 'jmp' instruction
				};				
				
				break;
			}
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
			case INST_IRET: {
				auto inst_1 = instruction::make_load(
					instruction::load_mode::MEMDIR,
					14,
					0,
					15,
					0
				);
				section->append(&inst_1, 4);

				auto inst_2 = instruction::make_arithmetic(
					instruction::arithmetic_operation::ADD,
					14,
					14,
					4
				);
				section->append(&inst_2, 4);

				auto inst_3 = instruction::make_load(
					instruction::load_mode::WRITE_CSR_MEMDIR,
					0,
					0,
					15,
					0
				);
				section->append(&inst_3, 4);

				auto inst_4 = instruction::make_arithmetic(
					instruction::arithmetic_operation::ADD,
					14,
					14,
					4
				);
				section->append(&inst_4, 4);

				break;
			}
			case INST_RET: {
				auto inst_1 = instruction::make_load(
					instruction::load_mode::MEMDIR,
					14,
					0,
					15,
					0
				);
				section->append(&inst_1, 4);

				auto inst_2 = instruction::make_arithmetic(
					instruction::arithmetic_operation::ADD,
					14,
					14,
					4
				);
				section->append(&inst_2, 4);

				break;
			}
			case INST_PUSH: {
				auto inst_1 = instruction::make_arithmetic(
					instruction::arithmetic_operation::SUB,
					14,
					14,
					4
				);
				section->append(&inst_1, 4);

				auto inst_2 = instruction::make_store(
					instruction::store_mode::MEMDIR,
					14,
					0,
					line.inst.params.reg1,
					0
				);
				section->append(&inst_2, 4);

				break;
			}
			case INST_POP: {
				auto inst_1 = instruction::make_load(
					instruction::load_mode::MEMDIR,
					14,
					0,
					line.inst.params.reg1,
					0
				);
				section->append(&inst_1, 4);

				auto inst_2 = instruction::make_arithmetic(
					instruction::arithmetic_operation::ADD,
					14,
					14,
					4
				);
				section->append(&inst_2, 4);

				break;
			}
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
				switch (line.inst.params.operand.type) {
				case operand::OPERAND_LITERAL_VALUE:
				case operand::OPERAND_SYMBOL_VALUE: {
					auto inst = instruction::make_load(
						instruction::load_mode::MEMDIR,
						line.inst.params.reg1,
						15,
						0,
						4
					);
					section->append(&inst, 4);

					if(line.inst.params.operand.type == operand::OPERAND_SYMBOL_VALUE) {
						auto symbol_name = line.inst.params.operand.symbol;
						auto symbol = symbol_table.find(symbol_name);
						if (!symbol)
							throw symbol_not_found_error(symbol_name);

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
					}
					
					section->append_literal(line.inst.params.operand.int_literal);
					break;
				}
				case operand::OPERAND_LITERAL_ADDR:
				case operand::OPERAND_SYMBOL_ADDR: {
					auto inst_1 = instruction::make_load(
						instruction::load_mode::MEMDIR,
						line.inst.params.reg1,
						15,
						0,
						4
					);
					section->append(&inst_1, 4);

					if(line.inst.params.operand.type == operand::OPERAND_SYMBOL_VALUE) {
						auto symbol_name = line.inst.params.operand.symbol;
						auto symbol = symbol_table.find(symbol_name);
						if (!symbol)
							throw symbol_not_found_error(symbol_name);

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
					}
					
					section->append_literal(line.inst.params.operand.int_literal);

					auto inst_2 = instruction::make_load(
						instruction::load_mode::MEMDIR,
						line.inst.params.reg1,
						line.inst.params.reg1,
						0,
						0
					);
					section->append(&inst_2, 4);

					break;
				}
				case operand::OPERAND_REG_VALUE: {
					auto inst = instruction::make_load(
						instruction::load_mode::REG_MOVE,
						line.inst.params.reg2,
						line.inst.params.reg1,
						0,
						0
					);
					section->append(&inst, 4);
					break;
				}
				case operand::OPERAND_REG_ADDR: {
					auto inst = instruction::make_load(
						instruction::load_mode::MEMDIR,
						line.inst.params.reg2,
						line.inst.params.reg1,
						0,
						0
					);
					section->append(&inst, 4);
					break;
				}
				case operand::OPERAND_REG_ADDR_WITH_LITERAL_OFFSET: {
					auto inst = instruction::make_load(
						instruction::load_mode::MEMDIR,
						line.inst.params.reg2,
						line.inst.params.reg1,
						0,
						line.inst.params.operand.int_literal
					);
					section->append(&inst, 4);
					break;
				}
				case operand::OPERAND_REG_ADDR_WITH_SYMBOL_OFFSET:
					throw std::runtime_error("Invalid operand type");
					// FIXME: Create a new exception type
				break;
			case INST_ST:
				switch (line.inst.params.operand.type) {
				case operand::OPERAND_LITERAL_VALUE:
				case operand::OPERAND_SYMBOL_VALUE:
					throw std::runtime_error("Invalid operand type");
					// FIXME: Create a new exception type
				case operand::OPERAND_LITERAL_ADDR:
				case operand::OPERAND_SYMBOL_ADDR: {
					auto inst = instruction::make_store(
						instruction::store_mode::MEMINDIR,
						15,
						0,
						line.inst.params.reg1,
						4
					);
					section->append(&inst, 4);

					if(line.inst.params.operand.type == operand::OPERAND_SYMBOL_VALUE) {
						auto symbol_name = line.inst.params.operand.symbol;
						auto symbol = symbol_table.find(symbol_name);
						if (!symbol)
							throw symbol_not_found_error(symbol_name);

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
					}
					
					section->append_literal(line.inst.params.operand.int_literal);
					break;
				}
				case operand::OPERAND_REG_VALUE: {
					auto inst = instruction::make_load(
						instruction::load_mode::REG_MOVE,
						line.inst.params.reg2,
						line.inst.params.reg1,
						0,
						0
					);
					section->append(&inst, 4);
					break;
				}
				case operand::OPERAND_REG_ADDR: {
					auto inst = instruction::make_store(
						instruction::store_mode::MEMDIR,
						line.inst.params.reg2,
						0,
						line.inst.params.reg1,
						0
					);
					section->append(&inst, 4);
					break;
				}
				case operand::OPERAND_REG_ADDR_WITH_LITERAL_OFFSET: {
					auto inst = instruction::make_store(
						instruction::store_mode::RELATIVE,
						line.inst.params.reg2,
						0,
						line.inst.params.reg1,
						line.inst.params.operand.int_literal
					);
					section->append(&inst, 4);
					break;
				}
				case operand::OPERAND_REG_ADDR_WITH_SYMBOL_OFFSET: 
					throw std::runtime_error("Invalid operand type");
					// FIXME: Create a new exception type
				break;
			case INST_CSRRD: {
				auto inst = instruction::make_load(
					instruction::load_mode::READ_CSR,
					line.inst.params.reg2,
					line.inst.params.reg1,
					0,
					0
				);
				section->append(&inst, 4);
				break;
			}
			case INST_CSRWR: {
				auto inst = instruction::make_load(
					instruction::load_mode::WRITE_CSR,
					line.inst.params.reg2,
					line.inst.params.reg1,
					0,
					0
				);
				section->append(&inst, 4);
				break;
			}
			}

			break;
		}
		}
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