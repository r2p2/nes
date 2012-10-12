#pragma once

#include "memory.h"

class MemoryMapper : public Memory
{
	struct MemMap
	{
		uint16_t offset;
		uint16_t size;
		Memory &memory;
	};

public:
	MemoryMapper()
	: Memory(0)
	{}

	void register_memory(uint16_t offset, uint16_t size, Memory &memory)
	{
		const MemMap mapping = {offset, size, memory};
		_mapping.push_back(mapping);
	}

	virtual Memory::mem_t read(uint16_t addr) const
	{ return find_mapping(addr).read(addr); }

	virtual void write(uint16_t addr, Memory::mem_t value)
	{ find_mapping(addr).write(addr, value); }

private:
	MemMap& find_mapping(uint16_t addr) const
	{
		std::vector<MemMap>::const_iterator it;
		for(it = _mapping.begin(); it != _mapping.end(); ++it)
		{
			const MemMap &mapping = *it;
			if(addr >= mapping.offset and
				addr < (mapping.offset + mapping.size))
				return mapping;
		}

		throw "no memory mapping found";
	}

private:
	std::vector<MemMap> _mapping;
};
