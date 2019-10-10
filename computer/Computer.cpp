#include "Computer.h"

#include <iostream>
#include <stdio.h>

#include "properties.h"
#include "opcode.h"


// The type for the extra instruction data (64 bits wide)
union extra_t
{
	// QWord
	uint64_t q;

	// DWords
	struct
	{
		uint32_t low,
			high;
	} d;

	// Words
	uint16_t w[4];

	// Bytes
	uint8_t b[8];
};


Computer::Computer(const uint8_t *PROGRAM_DATA, void (*onDebuggerTrap)())
	: prog(PROGRAM_DATA), onDebuggerTrap(onDebuggerTrap)
{}


uint16_t Computer::start()
{
	using namespace opcode;

	// Init RAM
	ram.data[0] = ram.data[1] = 0xFF;



	// Instruction opcode (id)
	uint8_t instruction;

	// The extra data with the instruction
	extra_t extra = { 0 };

	while (reg.c < MAX_PROGRAM_DATA_SIZE)
	{
		instruction = prog.data[reg.c];

		const size_t EXTRA_SIZE = (size_t)extraSizes[instruction];

		// Test for extra data
		switch (EXTRA_SIZE)
		{
		case 1:
			extra.b[0] = prog.data[reg.c + 1];
			break;

		case 2:
			extra.w[0] = *((uint16_t*)(prog.data + reg.c + 1));
			break;

		case 3:
		case 4:
			// The last byte is useless for the case 3
			extra.d.low = *((uint32_t*)(prog.data + reg.c + 1));
			break;
		}

		switch (instruction)
		{
			//////// Special ////////
		case EXIT:
			goto cpu_exit;

		case TRAP:
			if (onDebuggerTrap != nullptr)
				onDebuggerTrap();
			break;

			
			//////// Control Flow ////////
		case GO:
			// Go address
			reg.c = extra.w[0];

			// Don't update the program counter 
			continue;

		case GOEQ:
			if (reg.f & Registers::FLAGS::EQUAL)
			{
				// Go address
				reg.c = extra.w[0];

				// Don't update the program counter 
				continue;
			}
			break;

		case GONEQ:
			if (!(reg.f & Registers::FLAGS::EQUAL))
			{
				// Go address
				reg.c = extra.w[0];

				// Don't update the program counter 
				continue;
			}
			break;

		case GOS:
			if (reg.f & Registers::FLAGS::SMALLER)
			{
				// Go address
				reg.c = extra.w[0];

				// Don't update the program counter 
				continue;
			}
			break;

		case GOSEQ:
			if ((reg.f & Registers::FLAGS::SMALLER) | (reg.f & Registers::FLAGS::EQUAL))
			{
				// Go address
				reg.c = extra.w[0];

				// Don't update the program counter 
				continue;
			}
			break;

		case GOB:
			if (reg.f & Registers::FLAGS::BIGGER)
			{
				// Go address
				reg.c = extra.w[0];

				// Don't update the program counter 
				continue;
			}
			break;

		case GOBEQ:
			if ((reg.f & Registers::FLAGS::BIGGER) | (reg.f & Registers::FLAGS::EQUAL))
			{
				// Go address
				reg.c = extra.w[0];

				// Don't update the program counter 
				continue;
			}
			break;


			//////// Functions ////////
		case ENTER:
			// Next instruction
			push(reg.c + 3);

			// Previous stack frame address
			push(reg.r);

			// Update the old program counter pointer
			reg.r = reg.s - 4;

			// Go to the function
			reg.c = extra.w[0];

			// Don't update the program counter
			continue;

		case LEAVE:
			// Can't really leave, exit
			if (reg.r == 0)
				goto cpu_exit;

			// Remove stack frame
			reg.s = reg.r + 4;

			// Previous stack frame address
			reg.r = pop();

			// Next instruction
			reg.c = pop();

			// Don't update the program counter
			continue;


			//////// Registers ////////
			// x //
		case INC_X:
			++reg.x;
			break;

		case DEC_X:
			--reg.x;
			break;

		case SET_X_IMM:
			reg.x = extra.w[0];
			break;

		case SET_X_Y:
			reg.x = reg.y;
			break;

		case ADD_X_IMM:
			reg.x += extra.w[0];
			break;

		case ADD_X_Y:
			reg.x += reg.y;
			break;

		case SUB_X_IMM:
			reg.x -= extra.w[0];
			break;

		case SUB_X_Y:
			reg.x -= reg.y;
			break;

		case MUL_X_IMM:
			reg.x *= extra.w[0];
			break;

		case MUL_X_Y:
			reg.x *= reg.y;
			break;

		case TST_X_IMM:
			test(reg.x, extra.w[0]);
			break;

		case TST_X_Y:
			test(reg.x, reg.y);
			break;

		case PUSH_X:
			push(reg.x);
			break;

		case POP_X:
			reg.x = pop();
			break;

		case LOAD_X_IMM:
			reg.x = load(extra.w[0]);
			break;

		case SAVE_X_IMM:
			save(extra.w[0], reg.x);
			break;

		case PRINT_INT_X:
			std::cout << reg.x;
			break;

		case PRINT_CHR_X:
			std::cout << static_cast<char>(reg.x);
			break;



			// y //
		case INC_Y:
			++reg.y;
			break;

		case DEC_Y:
			--reg.y;
			break;

		case SET_Y_IMM:
			reg.y = extra.w[0];
			break;

		case SET_Y_X:
			reg.y = reg.x;
			break;

		case ADD_Y_IMM:
			reg.y += extra.w[0];
			break;

		case ADD_Y_X:
			reg.y += reg.x;
			break;

		case SUB_Y_IMM:
			reg.y -= extra.w[0];
			break;

		case SUB_Y_X:
			reg.y -= reg.x;
			break;
			
		case MUL_Y_IMM:
			reg.y *= extra.w[0];
			break;

		case MUL_Y_X:
			reg.y *= reg.x;
			break;

		case TST_Y_IMM:
			test(reg.y, extra.w[0]);
			break;

		case TST_Y_X:
			test(reg.y, reg.x);
			break;

		case PUSH_Y:
			push(reg.y);
			break;

		case POP_Y:
			reg.y = pop();
			break;

		case LOAD_Y_IMM:
			reg.y = load(extra.w[0]);
			break;

		case SAVE_Y_IMM:
			save(extra.w[0], reg.y);
			break;

		case PRINT_INT_Y:
			std::cout << reg.y;
			break;

		case PRINT_CHR_Y:
			std::cout << static_cast<char>(reg.y);
			break;


			//////// IO ////////
			// Print //
		case PRINT_NL:
			std::cout << '\n';
			break;

		default:
			fprintf(stderr, "Error : Illegal instruction with opcode 0x%02hhx, c = 0x%02hx\n", instruction, reg.c);
			return -1;
		}

		// The program counter is updated after the instruction
		// Increment the program counter by the size of the current instruction
		reg.c += static_cast<uint16_t>(EXTRA_SIZE) + 1;
	}

	// The program counter is outside of the program's memory
	std::cerr << "Error : Program counter overlow\n";
	return -1;

cpu_exit:

	return reg.x;
}

void Computer::test(const uint16_t a, const uint16_t b)
{
	// Clear flags
	reg.f = 0;

	if (a == b)
		reg.f |= Registers::FLAGS::EQUAL;

	if (a < b)
		reg.f |= Registers::FLAGS::SMALLER;
	else if (a > b)
		reg.f |= Registers::FLAGS::BIGGER;

}

void Computer::push(const uint16_t value)
{
	// Set value
	*(reinterpret_cast<uint16_t*>(ram.data + reg.s)) = value;

	// Update the stack pointer
	reg.s += 2;
}

uint16_t Computer::pop()
{
	// Update the stack pointer
	reg.s -= 2;

	// Get value
	return *(reinterpret_cast<uint16_t*>(ram.data + reg.s));
}

uint16_t Computer::load(const uint16_t OFFSET)
{
	// Address in the stack
	const size_t ADDRESS = (size_t)reg.r + OFFSET;

	// Handle exceptions
	if (ADDRESS > 0xFF)
	{
		// The target address is outside of the stack
		std::cerr << "Error : Stack overlow, can't load address outside of allocated stack memory\n";

		// Force exit	
		exit(-1);
	}

	return ram.data[ADDRESS];
}

void Computer::save(const uint16_t OFFSET, const uint16_t VALUE)
{
	// Address in the stack
	const size_t ADDRESS = (size_t)reg.r + OFFSET;

	// Handle exceptions
	if (ADDRESS > 0xFF)
	{
		// The target address is outside of the stack
		std::cerr << "Error : Stack overlow, can't save address outside of allocated stack memory\n";

		// Force exit	
		exit(-1);
	}

	// Set value
	ram.data[ADDRESS] = VALUE;
}
