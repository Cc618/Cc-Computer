#pragma once

// The opcode, don't forget to include cinttypes if uint8_t is missing

// !!! All instructions for registers with immediate value
// !!! have to be ordered the same way and must start with INC_REG

#ifdef __cplusplus
#define CONSTANT constexpr
#else
#define CONSTANT const
#endif

static CONSTANT uint8_t
	//////// Special ////////
	EXIT = 0x01,			// Exit program
	TRAP = 0x02,			// Traps the debugger


	//////// Control Flow ////////
	GO = 0x10,				// Equivalent for jmp
	GOEQ = 0x11,			// Equivalent for je, go if equal
	GONEQ = 0x12,			// Equivalent for jne, go if not equal
	GOS = 0x13,				// Equivalent for jl, go if smaller
	GOSEQ = 0x14,			// Equivalent for jle, go if smaller or equal
	GOB = 0x15,				// Equivalent for jg, go if bigger
	GOBEQ = 0x16,			// Equivalent for jge, go if bigger or equal


	//////// Functions ////////
	ENTER = 0x20,			// Equivalent for call and enter in x86, calls a function
	LEAVE = 0x21,			// Equivalent for leave and ret in x86, leaves the current function


	//////// Registers ////////
	// x //
	INC_X				= 0x30,		// Increment x
	DEC_X				= 0x31,		// Decrement x

	SET_X_IMM			= 0x32,		// Set x with immediate value
	SET_X_Y				= 0x33,		// Set x by the value of y

	ADD_X_IMM			= 0x34,		// Add to x an immediate value
	ADD_X_Y				= 0x35,		// Add to x the value of y

	SUB_X_IMM			= 0x36,		// Sub to x an immediate value
	SUB_X_Y				= 0x37,		// Sub to x the value of y

	MUL_X_IMM			= 0x38,		// Multiply x by a constant
	MUL_X_Y				= 0x39,		// Multiply x by y

	TST_X_IMM			= 0x3A,		// Equivalent for cmp with an immediate value
	TST_X_Y				= 0x3B,		// Equivalent for cmp with the y register

	PUSH_X				= 0x3C,		// Pushes x on the stack
	POP_X				= 0x3D,		// Pops the value on the top of the stack in x

	LOAD_X_IMM			= 0x3E,		// Load value from offset to r (return register) in x
	SAVE_X_IMM			= 0x3F,		// Save x from offset to r (return register)

	PRINT_INT_X			= 0x40,		// Print x as integer
	PRINT_CHR_X			= 0x41,		// Print LSB of x as character

	// y //
	INC_Y				= 0x50,		// Increment y
	DEC_Y				= 0x51,		// Decrement y

	SET_Y_IMM			= 0x52,		// Set y with immediate value
	SET_Y_X				= 0x53,		// Set y by the value of x

	ADD_Y_IMM			= 0x54,		// Add to y an immediate value
	ADD_Y_X				= 0x55,		// Add to y the value of x

	SUB_Y_IMM			= 0x56,		// Sub to y an immediate value
	SUB_Y_X				= 0x57,		// Sub to y the value of x

	MUL_Y_IMM			= 0x58,		// Multiply y by a constant
	MUL_Y_X				= 0x59,		// Multiply y by x

	TST_Y_IMM			= 0x5A,		// Equivalent for cmp with an immediate value
	TST_Y_X				= 0x5B,		// Equivalent for cmp with the x register

	PUSH_Y				= 0x5C,		// Pushes y on the stack
	POP_Y				= 0x5D,		// Pops the value on the top of the stack in y

	LOAD_Y_IMM			= 0x5E,		// Load value from offset to r (return register) in y
	SAVE_Y_IMM			= 0x5F,		// Save y from offset to r (return register)

	PRINT_INT_Y			= 0x60,		// Print y as integer
	PRINT_CHR_Y			= 0x61,		// Print LSB of y as character



	//////// IO ////////
	//// Print ////
	PRINT_NL = 0xC0			// Print a new line
	;

