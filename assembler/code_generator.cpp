#include "code_generator.h"

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <ios>
#include <cstring>
#include <ctype.h>

#include "main.h"
#include "properties.h"


// The generated code
std::vector<uint8_t> code;

// Labels are added when parsing
// After we remap go statements
std::list<label_t> labels;

// We resolve labels' addresses
// after the first code generation
// so we keep in memory go statements
std::list<go_instruction_t> gos;


// Push to code a word (in little endian)
inline void pushWord(const uint16_t WORD)
{
	code.push_back(static_cast<uint8_t>(WORD & 0x00FF));
	code.push_back(static_cast<uint8_t>((WORD & 0xFF00) >> 8));
}

// Returns the offset of the register's instruction set
// from the first register's (x) instruction set
inline size_t getRegisterOffset(const char ID)
{
	switch (ID)
	{
	case 'x':
		return INC_X - REGISTERS_OFFSET;
		break;

	case 'y':
		return INC_Y - REGISTERS_OFFSET;
		break;
	}

	return -1;
}


cfunc void genImmediateOperation(const immediate_operation_t OP)
{
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(OP.reg.id);

	// Instruction
	switch (OP.operand)
	{
	case '=':
		code.push_back(SET_X_IMM + REG_OFFSET);
		break;

	case '+':
		code.push_back(ADD_X_IMM + REG_OFFSET);
		break;

	case '-':
		code.push_back(SUB_X_IMM + REG_OFFSET);
		break;

	case '*':
		code.push_back(MUL_X_IMM + REG_OFFSET);
		break;

	default:
		yyerror("Invalid operand");
		break;
	}

	// Add data to code
	pushWord(OP.constant);
}

cfunc void genRegisterOperation(const register_operation_t OP)
{
	// To handle errors
	bool invalidRegister = false;

	// Instruction
	switch (OP.operand)
	{
	case '=':
		// x = y
		if (OP.target.id == 'x' && OP.value.id == 'y')
			code.push_back(SET_X_Y);
		// y = x
		else if (OP.target.id == 'y' && OP.value.id == 'x')
			code.push_back(SET_Y_X);
		else
			invalidRegister = true;
		break;

	case '+':
		// x += y
		if (OP.target.id == 'x' && OP.value.id == 'y')
			code.push_back(ADD_X_Y);
		// y += x
		else if (OP.target.id == 'y' && OP.value.id == 'x')
			code.push_back(ADD_Y_X);
		else
			invalidRegister = true;
		break;

	case '-':
		// x -= y
		if (OP.target.id == 'x' && OP.value.id == 'y')
			code.push_back(SUB_X_Y);
		// y -= x
		else if (OP.target.id == 'y' && OP.value.id == 'x')
			code.push_back(SUB_Y_X);
		else
			invalidRegister = true;
		break;

	case '*':
		// x *= y
		if (OP.target.id == 'x' && OP.value.id == 'y')
			code.push_back(MUL_X_Y);
		// y *= x
		else if (OP.target.id == 'y' && OP.value.id == 'x')
			code.push_back(MUL_Y_X);
		else
			invalidRegister = true;
		break;
	}

	if (invalidRegister)
	{
		semanticError = 1;
		yyerror("No instruction for these registers");
	}
}

cfunc void genUnaryOperation(const unary_operation_t OP)
{
	// Test register
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(OP.reg.id);

	// Instruction
	switch (OP.id)
	{
	case ID_INC:
		code.push_back(INC_X + REG_OFFSET);
		break;

	case ID_DEC:
		code.push_back(DEC_X + REG_OFFSET);
		break;

	default:
		yyerror("Invalid operand");
		break;
	}
}

cfunc void genInstruction(const uint8_t OPCODE)
{
	code.push_back(OPCODE);
}

cfunc void genGo(go_instruction_t go)
{
	if (go.goId.first == '.')
		// That's a function call (enter function)
		code.push_back(ENTER);

	else if (go.goId.second == '.')
		switch (go.goId.first)
		{
		case '@':
			// Simple go
			code.push_back(GO);
			break;
		
		case '=':
			code.push_back(GOEQ);
			break;
		
		case '!':
			code.push_back(GONEQ);
			break;

		case '<':
			code.push_back(GOS);
			break;

		case '>':
			code.push_back(GOB);
			break;
		}
	// The second char is '='
	else
		switch (go.goId.first)
		{
		case '<':
			code.push_back(GOSEQ);
			break;

		case '>':
			code.push_back(GOBEQ);
			break;
		}

	// Set address' offset and save it
	go.addressOffset = code.size();
	gos.push_back(go);

	// Reserve memory for the address
	code.push_back(0x00);
	code.push_back(0x00);
}

cfunc void genTestRegister(const test_register_instruction_t TEST)
{
	// true if there is no instruction matches the test
	bool invalidTest = false;

	if (TEST.first.id == (uint8_t)'x')
	{
		if (TEST.second.id == (uint8_t)'y')
			code.push_back(TST_X_Y);
		else
			invalidTest = true;
	}
	else if (TEST.first.id == (uint8_t)'y')
	{
		if (TEST.second.id == (uint8_t)'x')
			code.push_back(TST_Y_X);
		else
			invalidTest = true;
	}
	else
		invalidTest = true;

	// Check for error
	if (invalidTest)
		yyerror((std::string("Invalid test with registers '") + (char)TEST.first.id + "' and '" + (char)TEST.second.id + '\'').c_str());
}

cfunc void genTestImmediate(const test_immediate_instruction_t TEST)
{
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(TEST.first.id);

	// Test instruction
	code.push_back(TST_X_IMM + REG_OFFSET);

	// Test value
	pushWord(TEST.second);
}

cfunc void genPushRegister(const push_register_instruction_t PUSH)
{
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(PUSH.reg.id);

	// Add opcode
	code.push_back(PUSH_X + REG_OFFSET);
}

cfunc void genPopRegister(const pop_register_instruction_t POP)
{
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(POP.reg.id);

	// Add opcode
	code.push_back(POP_X + REG_OFFSET);
}

cfunc void genPrintRegister(const print_register_instruction_t i)
{
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(i.reg.id);
	
	switch (i.format)
	{
	case 'i':
		code.push_back(PRINT_INT_X + REG_OFFSET);
		break;

	case 'c':
		code.push_back(PRINT_CHR_X + REG_OFFSET);
		break;

	default:
		yyerror((std::string("Invalid format '") + i.format + '\'').c_str());
	}
}

cfunc void genLoadSaveInstruction(const load_save_instruction_t i)
{
	// The offset of the register
	const size_t REG_OFFSET = getRegisterOffset(i.reg.id);
	
	switch (i.type)
	{
		// Save
	case '>':
		code.push_back(SAVE_X_IMM + REG_OFFSET);
		break;

		// Load
	case '<':
		code.push_back(LOAD_X_IMM + REG_OFFSET);
		break;
	}

	pushWord(i.offset);
}

cfunc void registerLabel(label_t label)
{
	// Set the byte offset
	label.offset = code.size();

	// Add it to parsed labels
	labels.push_back(label);
}

cfunc int finalizeCode()
{
	// Return code
	int ret = 0;

	// Resolve go statements //
	for (const auto &g : gos)
	{
		// Search the target label
		const label_t *TARGET = nullptr;
		for (const label_t &label : labels)
			// Same ids
			if (strcmp(g.labelId, label.id) == 0)
			{
				// Set the TARGET and exit loop
				TARGET = &label;
				break;
			}
		
		// Label not found
		if (TARGET == nullptr)
		{
			yyerrorRaw((std::string("Invalid go statement, label '") + g.labelId + "' not found").c_str());
			ret = -1;
		}
		else
		{
			// Remap the instruction's address
			code[g.addressOffset] = static_cast<uint8_t>(TARGET->offset);
			code[g.addressOffset + 1] = static_cast<uint8_t>(TARGET->offset >> 8);
		}
	}	

	// Free every strings //
	// Labels
	for (const auto &l : labels)
		delete l.id;
	
	// Go statements
	for (const auto &g : gos)
		delete g.labelId;

	return ret;
}

cfunc void generateFile(const char *PATH)
{
	std::ofstream file(PATH, std::ios::binary);

	if (file.is_open())
	{
		// std::cout << "Writing file...\n";
		// std::cout << "Code size: " << code.size() << '\n';

		// Write the file
		file.write(reinterpret_cast<char*>(&code[0]), code.size());

		file.close();
		
		// // Display raw binary output
		// system("hd ../code.bin");

		// Call the python script to convert to human readable code
		// system((std::string("cd ..; python3 c_converter.py '")
		// 	+ BIN_NAME + '\'' /*+ "' '" + C_NAME + '\''*/).c_str());
		// system((std::string("cd ..; python3 c_converter.py '")
		// 	+ BIN_NAME + "' '" + C_NAME + '\'').c_str());
	}
	else
		std::cerr << "Error : Can't create generated binary file with name '" << PATH << "'\n";
}
