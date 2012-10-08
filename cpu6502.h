#pragma once

#include <stdint.h>

class CPU6502
{
public:
	CPU6502(Memory &memory)
	: _memory(memory)
	{
		reset();
	}
	
	void reset()
	{
		_reg_a  = 0;
		_reg_y  = 0;
		_reg_x  = 0;
		_pc     = 0;
		_sp     = 0;
		_reg_ps = 0;
	}

	void beat()
	{
	}

private:

	// external components
	Memory &_memory;

	// internal components
	uint8_t  _reg_a;  // accumulator
	uint8_t  _reg_y;  // index register
	uint8_t  _reg_x;  // index register
	uint16_t _pc;     // program counter
	uint8_t  _sp;     // stack pointer
	uint8_t  _reg_ps; // processor status register
};
