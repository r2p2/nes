#pragma once

#include <stdint.h>

#include "memory.h"

class Controller : public Memory::Handler
{
public:
	enum Button {
		A      = 0,
		B      = 1,
		SELECT = 2,
		START  = 3,
		UP     = 4,
		DOWN   = 5,
		LEFT   = 6,
		RIGHT  = 7,
	};

	Controller(Memory &memory, uint8_t ctr_nr)
	: _memory(memory)
	, _contr_addr(0x4016 + ctr_nr)
	, _button({0, 0, 0, 0, 0, 0, 0, 0})
	, _index(0)
	, _handler_active(false)
	, _connected(false)
	{}

	void press(Button btn_id)
	{ _button[btn_id] = true; }

	void release(Button btn_id)
	{ _button[btn_id] = false; }

	void connect()
	{ _connected = true; }

	void disconnect()
	{ _connected = false; }

	virtual void on_mem_read(uint16_t addr, Memory::mem_t value)
	{
		if(_handler_active)
			return;

		_handler_active = true;

		// 0x40 because it seems that some games
		// expect 0x41 if a button is pressed.
		_memory.write(0x40 | (!_connected << 1) | _button[_index]);
		_index = (_index + 1) % 8;

		_handler_active = false;
	}

	virtual void on_mem_write(uint16_t addr, Memory::mem_t value)
	{
		if(_handler_active)
			return;

		_handler_active = true;

		if(value & 0x01)
			_index = 0;

		_handler_active = false;
	}

private:
	Memory &_memory;
	Memory::mem_t _contr_addr;
	Button	_button[8];
	uint8_t _index;
	bool _handler_active;
	bool _connected;
};
