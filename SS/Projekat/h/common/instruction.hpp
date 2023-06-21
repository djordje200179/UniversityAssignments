#pragma once

#include <cstdint>
#include <ostream>

struct instruction {
	uint32_t code : 4 = 0;
	uint32_t mode : 4 = 0;

	uint32_t reg1 : 4 = 0;
	uint32_t reg2 : 4 = 0;
	uint32_t reg3 : 4 = 0;

	uint32_t displacement : 12 = 0;

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
								 uint8_t reg1, uint8_t reg2,
								 uint16_t displacement);
	static instruction make_jump(jump_mode mode, bool mem_indirect,
								 uint8_t reg1, uint8_t reg2,  uint8_t reg3,
								 uint16_t displacement);
	static instruction make_xchg(uint8_t reg1, uint8_t reg2);
	static instruction make_arithmetic(arithmetic_operation operation,
									   uint8_t reg1, uint8_t reg2, uint8_t reg3);
	static instruction make_logical(logical_operation operation,
									uint8_t reg1, uint8_t reg2, uint8_t reg3);
	static instruction make_shift(bool right,
								  uint8_t reg1, uint8_t reg2, uint8_t reg3);
	static instruction make_store(store_mode mode,
								  uint8_t reg1, uint8_t reg2, uint8_t reg3,
								  uint16_t displacement);
	static instruction make_load(load_mode mode,
								 uint8_t reg1, uint8_t reg2, uint8_t reg3, uint16_t displacement);

	friend std::ostream& operator<<(std::ostream& os, const instruction& instruction);
};

static_assert(sizeof(instruction) == 4, "instruction must be 4 bytes long");