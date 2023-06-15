#include "common/instruction.hpp"

instruction instruction::make_halt() {
	return instruction{.code = 0x0};
}

instruction instruction::make_int() {
	return instruction{.code = 0x1};
}

instruction instruction::make_call(bool mem_indirect, uint8_t reg1, uint8_t reg2, uint16_t displacement) {
	return instruction{
		.code = 0x2,
		.mode = mem_indirect,
		.reg1 = reg1,
		.reg2 = reg2,
		.displacement = displacement
	};
}

instruction instruction::make_jump(jump_mode mode, bool mem_indirect,
								   uint8_t reg1, uint8_t reg2, uint8_t reg3,
								   uint16_t displacement) {
	return instruction{
		.code = 0x3,
		.mode = static_cast<uint8_t>(mode),
		.reg1 = reg1,
		.reg2 = reg2,
		.reg3 = reg3,
		.displacement = displacement
	};
}

instruction instruction::make_xchg(uint8_t reg1, uint8_t reg2) {
	return instruction{
		.code = 0x4, 
		.reg2 = reg1, 
		.reg3 = reg2
	};
}

instruction instruction::make_arithmetic(arithmetic_operation operation,
										 uint8_t reg1, uint8_t reg2, uint8_t reg3) {
	return instruction{
		.code = 0x5,
		.mode = static_cast<uint8_t>(operation),
		.reg1 = reg1,
		.reg2 = reg2,
		.reg3 = reg3
	};
}

instruction instruction::make_logical(logical_operation operation,
									  uint8_t reg1, uint8_t reg2, uint8_t reg3) {
	return instruction{
		.code = 0x6,
		.mode = static_cast<uint8_t>(operation),
		.reg1 = reg1,
		.reg2 = reg2,
		.reg3 = reg3
	};
}

instruction instruction::make_shift(bool right,
									uint8_t reg1, uint8_t reg2, uint8_t reg3) {
	return instruction{
		.code = 0x7, 
		.mode = right,
		.reg1 = reg1,
		.reg2 = reg2,
		.reg3 = reg3
	};
}

instruction instruction::make_store(store_mode mode,
									uint8_t reg1, uint8_t reg2, uint8_t reg3,
									uint16_t displacement) {
	return instruction{
		.code = 0x8,
		.mode = static_cast<uint8_t>(mode),
		.reg1 = reg1,
		.reg2 = reg2,
		.reg3 = reg3,
		.displacement = displacement
	};
}

instruction instruction::make_load(load_mode mode,
								   uint8_t reg1, uint8_t reg2, uint8_t reg3,
								   uint16_t displacement) {
	return instruction{
		.code = 0x9,
		.mode = static_cast<uint8_t>(mode),
		.reg1 = reg1,
		.reg2 = reg2,
		.reg3 = reg3,
		.displacement = displacement
	};
}