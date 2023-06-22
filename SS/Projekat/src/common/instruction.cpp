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
	auto inst_mode = (uint8_t)mode | (mem_indirect << 3);

	return instruction{
		.code = 0x3,
		.mode = inst_mode,
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

std::ostream& operator<<(std::ostream& os, const instruction& instruction) {
	static std::string op_codes[] = {
		"halt",
		"int",
		"call",
		"jump",
		"xchg",
		"arith",
		"logic",
		"shift",
		"store",
		"load"
	};

	os << op_codes[instruction.code];

	switch (instruction.code) {
	case 0b0010: {
		os << ':';
		static std::string modes[] = {
			"direct",
			"indirect"
		};
		os << modes[instruction.mode];

		os << " to gpr[" << instruction.reg1 << "] + gpr[" << instruction.reg2 << "] + " << instruction.get_displacement();

		break;
	}
	case 0b0011: {
		os << ':';
		static std::string address_modes[] = {
			"direct",
			"indirect",
		};
		os << address_modes[(instruction.mode >> 3) & 0b1];

		os << " to gpr[" << instruction.reg1 << "] + " << instruction.get_displacement();

		switch (instruction.mode & 0b111) {
		case 0b001: {
			os << " if gpr[" << instruction.reg2 << "] == gpr[" << instruction.reg3 << "]";
			break;
		}
		case 0b010: {
			os << " if gpr[" << instruction.reg2 << "] != gpr[" << instruction.reg3 << "]";
			break;
		}
		case 0b011: {
			os << " if gpr[" << instruction.reg2 << "] > gpr[" << instruction.reg3 << "]";
			break;
		}
		}

		break;
	}
	case 0b0100: {
		os << " gpr[" << instruction.reg2 << "] <-> gpr[" << instruction.reg3 << "]";

		break;
	}
	case 0b0101: {
		os << ':';
		static std::string modes[] = {
			"add",
			"sub",
			"mul",
			"div"
		};
		os << modes[instruction.mode];

		os << " with gpr[" << instruction.reg2 << "] and gpr[" << instruction.reg3 << "] ";
		os << "to gpr[" << instruction.reg1 << "]";

		break;
	}
	case 0b0110: {
		os << ':';
		static std::string modes[] = {
			"not",
			"and",
			"or",
			"xor"
		};
		os << modes[instruction.mode];

		os << " with gpr[" << instruction.reg2 << ']';
		if (instruction.mode != 0b0000)
			os << " and gpr[" << instruction.reg3 << ']';
		os << " to gpr[" << instruction.reg1 << "]";

		break;
	}
	case 0b0111: {
		os << ':';
		static std::string modes[] = {
			"left",
			"right"
		};
		os << modes[instruction.mode];

		os << " with gpr[" << instruction.reg2 << "] and gpr[" << instruction.reg3 << "] ";
		os << "to gpr[" << instruction.reg1 << "]";

		break;
	}
	case 0b1000: {
		os << ':';
		
		switch (instruction.mode) {
		case 0b0000: {
			os << "direct";

			os << " from gpr[" << instruction.reg3 << "] ";
			os << "to gpr[" << instruction.reg1 << "] + gpr[" << instruction.reg2 << "] + " << instruction.get_displacement();

			break;
		}
		case 0b0010: {
			os << "indirect";

			os << " from gpr[" << instruction.reg3 << "] ";
			os << "to gpr[" << instruction.reg1 << "] + gpr[" << instruction.reg2 << "] + " << instruction.get_displacement();

			break;
		}
		case 0b0001: {
			os << "preinc";

			os << " from gpr[" << instruction.reg3 << "] ";
			os << "to gpr[" << instruction.reg1 << "] after incrementing by " << instruction.get_displacement();

			break;
		}
		}

		break;
	}
	case 0b1001: {
		os << ':';
		
		switch (instruction.mode) {
		case 0b0000: {
			os << "load_csr";
			os << " from csr[" << instruction.reg2 << "]";
			break;
		}
		case 0b0001: {
			os << "reg_move";
			os << " from gpr[" << instruction.reg2 << "] + " << instruction.get_displacement();

			break;
		}
		case 0b0010: {
			os << "direct";
			os << " from gpr[" << instruction.reg2 << "] + gpr[" << instruction.reg3 << "] + " << instruction.get_displacement();

			break;
		}
		case 0b0011: {
			os << "postinc";
			os << " from gpr[" << instruction.reg2 << "] before incrementing by " << instruction.get_displacement();

			break;
		}
		case 0b0100: {
			os << "store_csr";
			os << " from gpr[" << instruction.reg2 << ']';

			break;
		}
		case 0b0101: {
			os << "or_csr";
			os << " from csr[" << instruction.reg2 << "] | " << instruction.get_displacement();

			break;
		}
		case 0b0110: {
			os << "csr_direct";
			os << " from gpr[" << instruction.reg2 << "] + gpr[" << instruction.reg3 << "] + " << instruction.get_displacement();

			break;
		}
		case 0b0111: {
			os << "csr_postinc";
			os << " from gpr[" << instruction.reg2 << ']';
			os << " before incrementing by " << instruction.get_displacement();

			break;
		}
		}

		switch ((instruction.mode >> 2) & 0b1) {
		case 0b0: {
			os << " to gpr[" << instruction.reg1 << ']';

			break;
		}
		case 0b1: {
			os << " to csr[" << instruction.reg1 << ']';

			break;
		}
		}

		break;
	}
	}

	return os;
}