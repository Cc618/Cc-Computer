#pragma once

// The instruction set

#include <cinttypes>


// All ids
namespace opcode
{	
#include "../shared/raw_opcode.h"
};


// The size in bytes of each opcode extra data
// This is the size of the entire instruction minus one byte
extern uint8_t extraSizes[256U];


// Inits the sizes of each extra
void initOpcode();
