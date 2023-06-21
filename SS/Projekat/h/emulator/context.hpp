#pragma once

#include <map>
#include <cstdint>
#include "common/instruction.hpp"
#include <ostream>

struct context {
	uint32_t gprs[16];
	uint32_t& sp = gprs[14];
	uint32_t& pc = gprs[15];

	uint32_t csrs[3];
	uint32_t& status = csrs[0];
	uint32_t& handler = csrs[1];
	uint32_t& cause = csrs[2];

	std::map<uint32_t, uint8_t> memory;

	context() {
		pc = 0x40000000;
		status = 0;
		gprs[0] = 0;
	}

	uint32_t read_from(uint32_t address) const;
	instruction read_instruction();
	void write_to(uint32_t address, uint32_t value);

	friend std::ostream& operator<<(std::ostream& os, const context& context);
};