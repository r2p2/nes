#pragma once

#include <stdint.h>
#include <string>
#include <sstream>

class CPU6502
{
public:
	enum OpCodes
	{                                 // len | time
		ADC_IMMEDIATE   = 0x69,   //  2     2
		ADC_ZERO_PAGE   = 0x65,   //  2     3
		ADC_ZERO_PAGE_X = 0x75,   //  2     4
		ADC_ABSOLUTE    = 0x6D,   //  3     4
		ADC_ABSOLUTE_X  = 0x7D,   //  3     4 (+1 if page crossed)
		ADC_ABSOLUTE_Y  = 0x79,   //  3     4 (+1 if page crossed)
        	ADC_INDIRECT_X  = 0x61,   //  2     6
		ADC_INDIRECT_Y  = 0x71,   //  2     5 (+1 if page crossed)

		AND_IMMEDIATE   = 0x29,   //  2     2
		AND_ZERO_PAGE   = 0x25,   //  2     3
		AND_ZERO_PAGE_X = 0x35,   //  2     4
		AND_ABSOLUTE    = 0x2D,   //  3     4
		AND_ABSOLUTE_X  = 0x3D,   //  3     4 (+1 if page crossed)
		AND_ABSOLUTE_Y  = 0x39,   //  3     4 (+1 if page crossed)
        	AND_INDIRECT_X  = 0x21,   //  2     6
		AND_INDIRECT_Y  = 0x31,   //  2     5 (+1 if page crossed)

		ASL_ACCUMULATOR = 0x0A,   //  1     2
		ASL_ZERO_PAGE   = 0x06,   //  2     5
		ASL_ZERO_PAGE_X = 0x16,   //  2     6
		ASL_ABSOLUTE    = 0x0E,   //  3     6
		ASL_ABSOLUTE_X  = 0x1E,   //  3     7

	};

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

	std::string str() const
	{
		std::stringstream ss;

		ss << "+----+----+----+--------+----+-------+" << std::endl;
		ss << "|regA|regY|regX|   pc   | sp |CZIDBVN|" << std::endl;
		ss << "+----+----+----+--------+----+-------+" << std::endl;
		ss << "|" << std::hex << reg_a() << std::dec;
		ss << "|" << std::hex << reg_y() << std::dec;
		ss << "|" << std::hex << pc() << std::dec;
		ss << "|" << std::hex << carry_flag?1:0 << std::dec;
		ss << "|" << std::hex << zero_flag?1:0 << std::dec;
		ss << "|" << std::hex << interrupt_disable?1:0 << std::dec;
		ss << "|" << std::hex << decimal_mode_flag?1:0 << std::dec;
		ss << "|" << std::hex << break_command?1:0 << std::dec;
		ss << "|" << std::hex << overflow_flag?1:0 << std::dec;
		ss << "|" << std::hex << negative_flag?1:0 << std::dec;
		ss << "|" << std::endl;
		ss << "+----+----+----+--------+----+-------+" << std::endl;

		return ss.str();
	}

	void serialize(std::ostream &os)
	{
		os << _reg_a;
		os << _reg_y;
		os << _reg_x;
		os << _pc;
		os << _sp;
		os << _reg_ps;
	}

	void deserializ(std::istream &is)
	{
		os >> _reg_a;
		os >> _reg_y;
		os >> _pc;
		os >> _sp;
		os >> _reg_ps;
	}

	void beat()
	{
		const uint8_t instr = mem_read_next_pc();
		exec(instr);
	}

private:
	void exec(uint8_t instr)
	{
		switch(instr)
		{
		case ADC_IMMEDIATE:
			inst_adc(immediate(mem_read_move_8()));
			break;
		case ADC_ZERO_PAGE:
			inst_adc(zero_page(mem_read_move_8()));
			break;
		case ADC_ZERO_PAGE_X:
			inst_adc(zero_page_x(mem_read_move_8()));
			break;
		case ADC_ABSOLUTE:
			inst_adc(absolute(mem_read_move_16()));
			break;
		case ADC_ABSOLUTE_X:
			inst_adc(absolute_x(mem_read_move_16()));
			break;
		case ADC_ABSOLUTE_Y:
			inst_adc(absolute_y(mem_read_move_16()));
			break;
        	case ADC_INDIRECT_X:
			inst_adc(indexed_x(mem_read_move_8()));
			break;
		case ADC_INDIRECT_Y:
			inst_adc(indirect_y(mem_read_move_8()));
			break;

		/*******************************************************************/

		case AND_IMMEDIATE:
			inst_and(immediate(mem_read_move_8()));
			break;
		case AND_ZERO_PAGE:
			inst_and(zero_page(mem_read_move_8()));
			break;
		case AND_ZERO_PAGE_X:
			inst_and(zero_page_x(mem_read_move_8()));
			break;
		case AND_ABSOLUTE:
			inst_and(absolute(mem_read_move_16()));
			break;
		case AND_ABSOLUTE_X:
			inst_and(absolute_x(mem_read_move_16()));
			break;
		case AND_ABSOLUTE_Y:
			inst_and(absolute_y(mem_read_move_16()));
			break;
        	case AND_INDIRECT_X:
			inst_and(indexed_x(mem_read_move_8()));
			break;
		case AND_INDIRECT_Y:
			inst_and(indirect_y(mem_read_move_8()));
			break;

		/*******************************************************************/

		case ASL_ACCUMULATOR:
			inst_asl();
			break;
		case ASL_ZERO_PAGE:
			inst_asl(zero_page(mem_read_move_8()));
			break;
		case ASL_ZERO_PAGE_X:
			inst_asl(zero_page_x(mem_read_move_8()));
			break;
		case ASL_ABSOLUTE:
			inst_asl(absolute(mem_read_move_16()));
			break;
		case ASL_ABSOLUTE_X:
			inst_asl(absolute_x(mem_read_move_16()));
			break;



		default:
		}
	}

	/* instructions ************************************************************/

	void inst_adc(uint8_t val)
	{
		if(reg_a() & val & 0x80)
		{
			carry_flag(true);
			overflow_flag(true); // TODO: not sure if this is correct
		}
		else
		{
			carry_flag(false);
			overflow_flag(false); // TODO: not sure if this is correct

		}

		reg_a(reg_a() + val);
		
		if(reg_a() == 0)
			zero_flag(true);
		else
			zero_flag(false);

		if(reg_a() & 0x80)
			negative_flag(true);
		else
			negative_flag(false);
	}

	void inst_and(uint8_t val)
	{
		reg_a(reg_a() & val);
		
		if(reg_a() == 0)
			zero_flag(true);
		else
			zero_flag(false);

		if(reg_a() & 0x80)
			negative_flag(true);
		else
			negative_flag(false);

	}

	// performs shift operation on accumulator register
	void inst_asl()
	{
		carry_flag(reg_a() & 0x80);

		reg_a(reg_a() << 1 );

		zero_flag(reg_a() == 0);
		negative_flag(reg_a() & 0x80);
	}


	// performs shift operation on memory
	void inst_asl(uint8_t val)
	{
		uint8_t mem =
	}

	/* value address resolution ************************************************/

	// Argument is named aval because it no address and no value. Took both.

	uint8_t immediate(uint8_t aval) const
	{ return aval; }

	uint8_t zero_page(uint8_t aval) const
	{ return mem_read(static_cast<uint16_t>(aval)); }

	uint8_t zero_page_x(uint8_t aval) const
	{ return mem_read(static_cast<uint16_t(reg_x() + aval)); }

	uint8_t zero_page_y(uint8_t aval) const
	{ return mem_read(static_cast<uint16_t(reg_y() + aval)); }

	uint8_t relative(uint8_t aval) const
	{ return mem_read(pc() + aval); }

	uint8_t absolute(uint16_t aval) const
	{ return mem_read(aval); }

	uint8_t absolute_x(uint16_t aval) const
	{ return mem_read(aval + reg_x()); }

	uint8_t absolute_y(uint16_t aval) const
	{ return mem_read(aval + reg_y()); }

	uint8_t indirect_x(uint8_t aval) const
	{ return mem_read(mem_read(aval+reg_x())); }

	uint8_t indirect_y(uint8_t aval) const
	{ return mem_read(mem_read(aval+reg_y())); }

	/* memory access ***********************************************************/

	// Read content from memory where pc is pointing to.
	// Increment pc.
	uint8_t mem_read_move_8()
	{
		const uint8_t mem = mem_read(pc());
		pc(pc()+1);
		return mem;
	}

	uint16_t mem_read_move_16()
	{
		uint16_t mem = mem_read(pc()) << 8;
		mem |= mem_read(pc()+1);
		pc(pc()+2);
		return mem;
	}

	uint8_t mem_read(uint16_t addr) const
	{ return memory.read(addr); }

	void mem_write(uint16_t addr, uint8_t val)
	{ memory.write(addr, val); }

	/* register access *********************************************************/

	uint8_t reg_a() const
	{ return _reg_a; }

	void reg_a(uint8_t val)
	{ _reg_a = val; }

	uint8_t reg_y() const
	{ return _reg_y; }

	void reg_y(uint8_t val)
	{ _reg_y = val; }

	uint8_t reg_x() const
	{ return _reg_x; }

	void reg_x(uint8_t val)
	{ _reg_x = val; }

	uint16_t pc() const
	{ return _pc; }

	void pc(uint16_t val)
	{ _pc = val; }

	uint8_t sp() const
	{ return _sp; }

	void sp(uint8_t val)
	{ _sp = val; }

	uint8_t reg_ps() const
	{ return _reg_ps; }

	void reg_ps(uint8_t val)
	{ _reg_ps = val; }

	/* processor status regist access ******************************************/
	
	bool carry_flag() const
	{ return reg_ps() & 0x40; }

	void carry_flag(bool val)
	{ reg_ps(reg_ps() | 0x40); }

	bool zero_flag() const
	{ return reg_ps() & 0x20; }

	void zero_flag(bool val)
	{ reg_ps(reg_ps() | 0x20); }

	bool interrupt_disable() const
	{ return reg_ps() & 0x10; }

	void interrupt_disable(bool val)
	{ reg_ps(reg_ps() | 0x10); }

	bool decimal_mode_flag() const
	{ return reg_ps() & 0x08; }

	void decimal_mode_flag(bool val)
	{ reg_ps(reg_ps() | 0x08); }

	bool break_command() const
	{ return reg_ps() & 0x04; }

	void break_command(bool val)
	{ return reg_ps(reg_ps() | 0x04); }

	bool overflow_flag() const
	{ return reg_ps() & 0x02; }

	void overflow_flag(bool val)
	{ return reg_ps(reg_ps() | 0x02); }

	bool negative_flag() const
	{ return reg_ps() & 0x01; }

	void negative_flag(bool val)
	{ return reg_ps(reg_ps() | 0x01); }

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