#pragma once

#include <stdint.h>
#include <vector>

class Memory
{
public:
	typedef uint8_t mem_t

	Memory(uint16_t size)
	: _memory(0xFFFF, 0)
	{}

	mem_t read(uint16_t addr) const
	{ return _memory[addr]; }

	void write(uint16_t addr, mem_t value)
	{ _memory[addr] = value; }

private:
	std::vector<mem_t> _memory;
};
