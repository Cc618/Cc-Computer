#pragma once

// Used to generate the binary code

#include "types.h"


#ifdef __cplusplus
#	define cfunc extern "C"
#else
#	define cfunc
#endif


// register operand constant
cfunc void genImmediateOperation(const immediate_operation_t OP);

// register operand register
cfunc void genRegisterOperation(const register_operation_t OP);

// register++
cfunc void genUnaryOperation(const unary_operation_t OP);

// Direct opcode appening
cfunc void genInstruction(const uint8_t OPCODE);

// label(id)
cfunc void genGo(const go_instruction_t go);

// register ? register
cfunc void genTestRegister(const test_register_instruction_t TEST);

// register ? constant
cfunc void genTestImmediate(const test_immediate_instruction_t TEST);

// > register
cfunc void genPushRegister(const push_register_instruction_t PUSH);

// < register
cfunc void genPopRegister(const pop_register_instruction_t POP);

// $f register with f the format (either i or c)
cfunc void genPrintRegister(const print_register_instruction_t i);

// register :> constant
cfunc void genLoadSaveInstruction(const load_save_instruction_t i);

// Appends a label to labels' memory
cfunc void registerLabel(label_t label);


// Make the last pass, remap go statements...
cfunc int finalizeCode();

// Generate the binary file
cfunc void generateFile(const char *PATH);
