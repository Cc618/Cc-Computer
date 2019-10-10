#include "opcode.h"

#include <cinttypes>


uint8_t extraSizes[256U];


void initOpcode()
{
	using namespace opcode;

	//// Special ////
	extraSizes[EXIT] = 0;
	extraSizes[TRAP] = 0;

	//// Control Flow ////
	extraSizes[GO] = extraSizes[GOEQ] = extraSizes[GONEQ] =
		extraSizes[GOS] = extraSizes[GOSEQ] = extraSizes[GOB] = 
		extraSizes[GOBEQ] = 2;

	//// Functions ////
	extraSizes[ENTER] = 2;
	extraSizes[LEAVE] = 0;

	//// Registers ////
	extraSizes[INC_X]		= extraSizes[INC_Y] = 0;
	extraSizes[DEC_X]		= extraSizes[DEC_Y] = 0;

	extraSizes[SET_X_IMM]	= extraSizes[SET_Y_IMM] = 2;
	extraSizes[SET_X_Y]		= extraSizes[SET_Y_X] = 0;
	
	extraSizes[ADD_X_IMM]	= extraSizes[ADD_Y_IMM] = 2;
	extraSizes[ADD_X_Y]		= extraSizes[ADD_Y_X] = 0;
	
	extraSizes[SUB_X_IMM]	= extraSizes[SUB_Y_IMM] = 2;
	extraSizes[SUB_X_Y]		= extraSizes[SUB_Y_X] = 0;
	
	extraSizes[MUL_X_IMM]	= extraSizes[MUL_Y_IMM] = 2;
	extraSizes[MUL_X_Y]		= extraSizes[MUL_Y_X] = 0;
	
	extraSizes[TST_X_IMM]	= extraSizes[TST_Y_IMM] = 2;
	extraSizes[TST_X_Y]		= extraSizes[TST_Y_X] = 0;
	
	extraSizes[PUSH_X]		= extraSizes[PUSH_Y] = 0;
	extraSizes[POP_X]		= extraSizes[POP_Y] = 0;
	
	extraSizes[LOAD_X_IMM]	= extraSizes[LOAD_Y_IMM] = 2;
	extraSizes[SAVE_X_IMM]	= extraSizes[SAVE_Y_IMM] = 2;
	
	extraSizes[PRINT_INT_X]	= extraSizes[PRINT_INT_Y] = 0;

	//// IO ////
	// Print //
	extraSizes[PRINT_NL] = 0;

}
