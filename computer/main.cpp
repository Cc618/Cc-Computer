#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <cstring>

#include "Computer.h"
#include "opcode.h"
#include "Debug.h"


using namespace opcode;


Computer* pc = nullptr;


/*
#define USE_INLINE_PROGRAM_DATA

// The program's data
const uint8_t PROGRAM_DATA[] = {
	SET_X_IMM, 6, 00,
	SET_Y_IMM, 8, 00,

	SAVE_X_IMM, 4, 00,
	INC_X,
	SAVE_X_IMM, 8, 00,

	SAVE_Y_IMM, 0x14, 00,
	DEC_Y,
	SAVE_Y_IMM, 0x18, 00,

	TRAP,
	LEAVE
};
*/

// When the computer receive the instruction trap, it goes here
void onDebuggerTrap()
{
	debug::log("-------- Debugger trapped --------");

	// Registers
	debug::logRegisters(*pc);

	// Stack
	debug::logStack(4, *pc);

	std::cout << '\n';
}


int main(int argc, char **argv)
{
#ifndef USE_INLINE_PROGRAM_DATA
	//// Arguments ////
	// Check arguments
	if (argc != 2)
	{
		std::cerr << "Usage : cc_computer <code_path>\n";

		return -1;
	}
	
	// Help message
	if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
	{
		std::cout << "Usage : cc_computer <code_path>\n";

		std::cout << "This program emulates a tiny computer, "
			"to generate a code file use Cc Assembler (ccasm)\n";

		return 0;
	}


	//// IO ////
	// Open the file
	std::ifstream file(argv[1], std::ios::binary);

	// Check for errors
	if (!file.is_open())
	{
		std::cerr << "Error : Can't open file\n";
		return -1;
	}

	// Read CRLFs
	file.unsetf(std::ios::skipws);

	// Read the code
	auto code = std::vector<uint8_t>(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	);

	file.close();

	// Check for errors
	if (code.size() == 0)
	{
		std::cerr << "Error : Can't execute an empty code file\n";

		return -1;
	}
#endif

	//// Compute ////
	// Init
	initOpcode();

	// PC
#ifdef USE_INLINE_PROGRAM_DATA
	pc = new Computer(PROGRAM_DATA, onDebuggerTrap);
#else
	pc = new Computer(&code[0], onDebuggerTrap);
#endif

	uint16_t ret = pc->start();


	// Return
	debug::log("--- Return ---");
	debug::logWord(ret);


	// Exit
	std::cout << '\n';

	delete pc;

	return ret;
}
