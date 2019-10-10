#pragma once

// To handle static and dynamic memory

#include <cinttypes>

#include "properties.h"


// Random access memory, read/write
struct RAM
{
	RAM();

	char data[RAM_SIZE];
};


// Program's memory, only read
struct Program
{
	Program();
	Program(const uint8_t* data);

	const uint8_t* data = nullptr;
};
