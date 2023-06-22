#include "emulator/context.hpp"
#include "common/instruction.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "common/utils.hpp"
#include <iostream>

void emulate(context& context) {
	auto running = true;
	bool interrupt = false;

	while (running) {
		auto instruction = context.read_instruction();

		//if (interrupt)
		//	std::cout << instruction << std::endl;

		switch (instruction.code) {
		case 0b0000: {
			running = false;
			break;
		}
		case 0b0001: {
			context.sp -= 4;
			context.write_to(context.sp, context.status);

			context.sp -= 4;
			context.write_to(context.sp, context.pc);

			context.cause = 4;
			context.status &= ~0x1;
			context.pc = context.handler;

			break;
		}
		case 0b0010: {
			switch(instruction.mode) {
			case 0b0000: {
				context.sp -= 4;
				context.write_to(context.sp, context.pc);

				context.pc = context.gprs[instruction.reg1] + context.gprs[instruction.reg2] + instruction.get_displacement();

				break;
			}
			case 0b0001: {
				context.sp -= 4;
				context.write_to(context.sp, context.pc);

				auto address = context.gprs[instruction.reg1] + context.gprs[instruction.reg2] + instruction.get_displacement();
				context.pc = context.read_from(address);

				break;
			}
			default:
				throw std::runtime_error("Invalid operation");
			}
			break;
		}
		case 0b0011: {
			auto address = context.gprs[instruction.reg1] + instruction.get_displacement(); 
			if (instruction.mode & 0b1000)
				address = context.read_from(address);

			switch (instruction.mode & 0b111) {
			case 0b000: {
				context.pc = address;
				break;
			}
			case 0b001: {
				if (context.gprs[instruction.reg2] == context.gprs[instruction.reg3])
					context.pc = address;

				break;
			}
			case 0b010: {
				if (context.gprs[instruction.reg2] != context.gprs[instruction.reg3])
					context.pc = address;

				break;
			}
			case 0b011: {
				if ((int)context.gprs[instruction.reg2] > (int)context.gprs[instruction.reg3])
					context.pc = address;

				break;
			}
			default:
				throw std::runtime_error("Invalid operation");
			}
			
			break;
		}
		case 0b0100: {
			std::swap(context.gprs[instruction.reg2], context.gprs[instruction.reg1]);

			break;
		}
		case 0b0101: {
			switch (instruction.mode) {
			case 0b0000: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] + context.gprs[instruction.reg3];

				break;
			}
			case 0b0001: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] - context.gprs[instruction.reg3];
				
				break;
			}
			case 0b0010: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] * context.gprs[instruction.reg3];
				
				break;
			}
			case 0b0011: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] / context.gprs[instruction.reg3];
				
				break;
			}
			default:
				throw std::runtime_error("Invalid operation");
			}
			break;
		}
		case 0b0110: {
			switch (instruction.mode) {
			case 0b0000: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = ~context.gprs[instruction.reg2];
				
				break;
			}
			case 0b0001: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] & context.gprs[instruction.reg3];
				
				break;
			}
			case 0b0010: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] | context.gprs[instruction.reg3];
				
				break;
			}
			case 0b0011: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] ^ context.gprs[instruction.reg3];
				
				break;
			}
			default:
				throw std::runtime_error("Invalid operation");
			}

			break;
		}
		case 0b0111: {
			switch (instruction.mode) {
			case 0b0000: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] << context.gprs[instruction.reg3];
				
				break;
			}
			case 0b0001: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] >> context.gprs[instruction.reg3];
				
				break;
			}
			default:
				throw std::runtime_error("Invalid operation");
			}

			break;
		}
		case 0b1000: {
			switch (instruction.mode) {
			case 0b0000: {
				auto address = context.gprs[instruction.reg1] + context.gprs[instruction.reg2] + instruction.get_displacement();
				context.write_to(address, context.gprs[instruction.reg3]);

				break;
			}
			case 0b0010: {
				auto address_1 = context.gprs[instruction.reg1] + context.gprs[instruction.reg2] + instruction.get_displacement();
				auto address_2 = context.read_from(address_1);
				context.write_to(address_2, context.gprs[instruction.reg3]);

				break;
			}
			case 0b0001: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] += instruction.get_displacement();

				context.write_to(context.gprs[instruction.reg1], context.csrs[instruction.reg3]);

				break;
			}

			default:
				throw std::runtime_error("Invalid operation");
			}

			break;
		}
		case 0b1001: {
			switch (instruction.mode) {
			case 0b0000: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.csrs[instruction.reg2];
				
				break;
			}
			case 0b0001: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.gprs[instruction.reg2] + instruction.get_displacement();
				
				break;
			}
			case 0b0010: {
				auto address = context.gprs[instruction.reg2] + context.gprs[instruction.reg3] + instruction.get_displacement();
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.read_from(address);

				break;
			}
			case 0b0011: {
				if (instruction.reg1 != 0)
					context.gprs[instruction.reg1] = context.read_from(context.gprs[instruction.reg2]);
				if (instruction.reg2 != 0)
					context.gprs[instruction.reg2] += instruction.get_displacement();

				break;
			}
			case 0b0100: {
				context.csrs[instruction.reg1] = context.gprs[instruction.reg2];
				break;
			}
			case 0b0101: {
				context.csrs[instruction.reg1] = context.csrs[instruction.reg2] | instruction.get_displacement();
				break;
			}
			case 0b0110: {
				auto address = context.gprs[instruction.reg2] + context.gprs[instruction.reg3] + instruction.get_displacement();
				context.csrs[instruction.reg1] = context.read_from(address);

				break;
			}
			case 0b0111: {
				context.csrs[instruction.reg1] = context.read_from(context.gprs[instruction.reg2]);
				if (instruction.reg2 != 0)
					context.gprs[instruction.reg2] += instruction.get_displacement();

				break;
			}
			default:
				throw std::runtime_error("Invalid operation");
			}

			break;
		}
		default:
			throw std::runtime_error("Invalid operation");
		}
	
		char ch;
		if (read(STDIN_FILENO, &ch, 1) >= 0) {
			if (context.status & 0b011)
				continue;
			
			interrupt = true;

			context.sp -= 4;
			context.write_to(context.sp, context.pc);
			context.sp -= 4;
			context.write_to(context.sp, context.status);

			context.status &= ~0x1;
			context.cause = 3;
			context.pc = context.handler;
			context.write_to(0xFFFFFF04, ch);
		}
	}
}