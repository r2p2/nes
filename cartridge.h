#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "memory.h"

class Cartridge
{
public:
	enum Mirroring
	{
		HORIZONRAL = 0,
		VERTICAL = 1,
	};

	enum MemoryMapper
	{
		NoMapper = 0,
	};

	enum DisplayFormat
	{
		NTSC = 0,
		PAL = 1,
	};

	Cartridge()
	{}

	void load(std::istream &is)
	{
		uint8_t format[4];
		uint8_t byte;

		is >> format;

		// TODO check format

		is >> byte;
		_rom.resize(16*byte, 0);

		is >> byte;
		_vrom.resize(8*byte, 0);

		is >> byte;
		_mirroring = byte & 0x01;
		_battery_backed_ram = byte & 0x02;
		if(byte & 0x04)
			_trainer.resize(512, 0);
		_four_screen_layout = byte & 0x08;
		_memory_mapper = byte >> 4;

		is >> byte;
		_vs_cartridge = byte & 0x01;
		_memory_mapper |= byte & 0xF0;
		
		is >> byte;
		if(bytes == 0)
			_8k_ram_banks = 1;
		else
			_8k_ram_banks = bytes;

		
		is >> byte;
		_display_format = byte & 0x01;

		is >> byte;
		is >> byte;
		is >> byte;
		is >> byte;
		is >> byte;
		is >> byte;

		for(uint16_t i = 0; i < _trainer.size(); ++i)
		{
			is >> byte;
			_trainer[i] = byte;
		}
		
		for(uint16_t i = 0; i < _rom.size(); ++i)
		{
			is >> byte;
			_rom[i] = byte;
		}

		for(uint16_t i = 0; i < _vram.size(); ++i)
		{
			is >> byte;
			_vram[i] = byte;
		}
	}

	std::string str() const
	{
		std::stringstream ss;

		ss << "mirroring: " << _mirroring << std::endl;
		ss << "battery backed ram: " << _battery_backed_ram << std::endl;
		ss << "four screen layout: " << _four_screen_layout << std::endl;
		ss << "memory mapper: " << _memory_mapper << std::endl; 
		ss << "vs cartridge: " << _vs_cartridge << std::endl;
		ss << "8k ram banks: " << _8k_ram_banks << std::endl;
		ss << "display format: " << _display_format << std::endl;
		ss << "trainer: " << _trainer.size() << std::endl;
		ss << "rom: " << _rom.size() << std::endl;
		ss << "vrom: " << _vrom.size() << std::endl;

		return ss.str();
	}

private:
	Mirroring _mirroring;
	bool _battery_backed_ram;
	bool _four_screen_layout;
	MemoryMapper _memory_mapper;
	bool _vs_cartridge;
	uint16_t _8k_ram_banks;
	DisplayFormat _display_format;
	std::vector<Memory::mem_t> _trainer;
	std::vector<Memory::mem_t> _rom;
	std::vector<Memory::mem_t> _vrom;
};
