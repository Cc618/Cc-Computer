#pragma once

// All registers

#include <cinttypes>


struct Registers
{
	// (enum - like) Flags for the f register
	struct FLAGS
	{
		static const uint8_t 
			EQUAL		= 1 << 0,
			BIGGER		= 1 << 1,
			SMALLER		= 1 << 2
			;
	};


	Registers();


	// General purpose
	uint16_t x = 0;
	uint16_t y = 0;
	
	// General purpose or parameters
	//uint16_t a = 0;
	//uint16_t b = 0;

	// Pointers
	//uint16_t p = 0;
	
	// Stack register
	uint16_t s = 4;
	// Return register
	uint16_t r = 0;

	// Flags (read only)
	uint8_t f = 0;

	// Program counter
	uint16_t c = 0;
};

