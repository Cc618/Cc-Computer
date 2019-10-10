#pragma once

#ifdef cplusplus
extern "C" {
#endif


#include <stdint.h>


#define ID_INC '+'
#define ID_DEC '-'


// A constant value
typedef uint16_t value_t;

// An operator
typedef char operand_t;
typedef operand_t unary_operand_t;

// An identifier (name)
typedef char* identifier_t;

// The number which formats the print
typedef char print_format_t;


// Register
typedef struct
{
	uint8_t id;
} reg_t;

// An operation for a register without other data
typedef struct
{
	reg_t reg;

	// + - ... (use definitions)
	unary_operand_t id;
} unary_operation_t;

// An operation for a register with a constant (immediate) value
typedef struct
{
	reg_t reg;

	// = + * ...
	operand_t operand;

	value_t constant;
} immediate_operation_t;

// An operation for a register with another register
typedef struct
{
	reg_t target;

	// = + * ...
	char operand;

	reg_t value;
} register_operation_t;

typedef struct
{
	// Name
	identifier_t id;

	// Offset from the start of the program in bytes
	uint16_t offset;
} label_t;

// When we emit a test with two registers
typedef struct
{
	reg_t first;
	reg_t second;
} test_register_instruction_t;

// A test with one register and a constant
typedef struct
{
	reg_t first;
	value_t second;
} test_immediate_instruction_t;

// The part in parenthesis in a go statement
typedef struct
{
	// Is '.' when it's a single quote
	char first;

	// For <=...
	char second;
} go_id_t;

// Describes a go statement
typedef struct
{
	// The label to go
	identifier_t labelId;

	// The go instruction id (for <, =...)
	go_id_t goId;

	// The offset of the address to resolve
	uint16_t addressOffset;
} go_instruction_t;

// Describes a push statement with a register
typedef struct
{
	// The target register
	reg_t reg;
} push_register_instruction_t;

// Describes a push statement with a constant
typedef struct
{
	// The target value
	uint16_t value;
} push_immediate_instruction_t;

// Describes a pop statement with a register
typedef struct
{
	// The target register
	reg_t reg;
} pop_register_instruction_t;

// Describes a pop statement with a constant
typedef struct
{
	// The target value
	uint16_t value;
} pop_immediate_instruction_t;

// Describes a print statement with a register
typedef struct
{
	char format;

	// The target register
	reg_t reg;
} print_register_instruction_t;

// Describes a save or load instruction
typedef struct
{
	// Target 
	reg_t reg;

	// Either < to load or > to save
	char type;

	// The offset from the return register
	uint16_t offset;
} load_save_instruction_t;



#ifdef cplusplus
}
#endif
