#pragma once

#include <cstdint>

struct instruction {
	uint8_t code : 4 = 0;
	uint8_t mode : 4 = 0;

	uint8_t reg1 : 4 = 0;
	uint8_t reg2 : 4 = 0;
	uint8_t reg3 : 4 = 0;

	uint16_t displacement : 12 = 0;

	enum struct jump_mode : uint8_t {
		UNCONDITIONAL,
		EQUAL,
		NOT_EQUAL,
		GREATER
	};
	enum struct arithmetic_operation : uint8_t { ADD, SUB, MUL, DIV };
	enum struct logical_operation : uint8_t { NOT, AND, OR, XOR };
	enum struct store_mode : uint8_t { MEMDIR, MEMINDIR, RELATIVE };
	enum struct load_mode : uint8_t {
		READ_CSR,
		REG_MOVE,
		MEMDIR,
		MEMDIR_POSTINC,
		WRITE_CSR,
		OR_CSR,
		WRITE_CSR_MEMDIR,
		WRITE_CSR_MEMDIR_POSTINC,
	};

	static instruction make_halt();
	static instruction make_int();
	static instruction make_call(bool mem_indirect,
								 uint8_t reg1,
								 uint8_t reg2,
								 uint16_t displacement);
	static instruction make_jump(enum jump_mode mode,
								 bool mem_indirect,
								 uint8_t reg1,
								 uint8_t reg2,
								 uint8_t reg3,
								 uint16_t displacement);
	static instruction make_xchg(uint8_t reg1, uint8_t reg2);
	static instruction make_arithmetic(enum arithmetic_operation operation,
									   uint8_t reg1,
									   uint8_t reg2,
									   uint8_t reg3);
	static instruction make_logical(enum logical_operation operation,
									uint8_t reg1,
									uint8_t reg2,
									uint8_t reg3);
	static instruction make_shift(bool right,
								  uint8_t reg1,
								  uint8_t reg2,
								  uint8_t reg3);
	static instruction make_store(enum store_mode mode,
								  uint8_t reg1,
								  uint8_t reg2,
								  uint8_t reg3,
								  uint16_t displacement);
	static instruction make_load(enum load_mode mode,
								 uint8_t reg1,
								 uint8_t reg2,
								 uint8_t reg3,
								 uint16_t displacement);
};