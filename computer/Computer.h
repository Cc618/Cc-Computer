#pragma once

// Main class
// This class is like a cpu but handles also other components

#include <cinttypes>

#include "Memory.h"
#include "Registers.h"


struct Computer
{
	Computer(const uint8_t *PROGRAM_DATA, void (*onDebuggerTrap)());

	// Launches the program
	uint16_t start();

	RAM ram;
	Program prog;
	Registers reg;

	void (*onDebuggerTrap)() = nullptr;


private:
	// Do the test instruction with a and b
	void test(const uint16_t a, const uint16_t b);

	// Pushes a word on the stack
	void push(const uint16_t value);

	// Pops the value on the top of the stack
	uint16_t pop();

	// Loads the value on the stack from the given offset to r (return register)
	uint16_t load(const uint16_t OFFSET);

	// Saves the value on the stack from the given offset to r (return register)
	void save(const uint16_t OFFSET, const uint16_t VALUE);
};

