#pragma once

#include <stdint.h>
#include <vector>
#include <map>

class Memory
{
public:
	typedef uint8_t mem_t;

	Memory(uint16_t size)
	: _memory(0xFFFF, 0)
	{}

	virtual mem_t read(uint16_t addr) const
	{ return _memory[addr]; }

	virtual void write(uint16_t addr, mem_t value)
	{ _memory[addr] = value; }

private:
	std::vector<mem_t> _memory;
};
