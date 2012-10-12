#pragma once

#include <stdint.h>
#include <vector>
#include <map>

class Memory
{
public:
	typedef uint8_t mem_t;
	class Handler
	{
		virtual void on_mem_read(uint16_t addr, mem_t value) = 0;
		virtual void on_mem_write(uint16_t addr, mem_t value) = 0;
	};


	Memory(uint16_t size)
	: _memory(0xFFFF, 0)
	{}

	virtual mem_t read(uint16_t addr) const
	{
		const mem_t mem = _memory[addr];

		if(_handlers.find(addr) != std::map<uint16_t, Handler*>::end())
			_handlers[addr].on_mem_read(addr, mem);

		return mem;
	}

	virtual void write(uint16_t addr, mem_t value)
	{
		_memory[addr] = value;

		if(_handlers.find(addr) != std::map<uint16_t, Handler*>::end())
			_handlers[addr].on_mem_write(addr, value);
	}

	void reg_mem_handler(uint16_t addr, Handler *hdl)
	{ _handlers[addr] = hdl; }

private:
	std::vector<mem_t> _memory;
	std::map<uint16_t, Handler*> _handlers;
};
