%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_generator.h"
#include "../shared/raw_opcode.h"


void yyerror(char *s);
int yylex();

%}

/* Types */
%code requires {
	#include "types.h"
}


%union {
	uint8_t cmd;
	uint8_t push_pop;
	char load_save_token;

	operand_t operand;
	unary_operand_t unary_operand;
	value_t value;
	reg_t reg;
	identifier_t identifier;
	go_id_t go_id;
	print_format_t print_format;

	unary_operation_t operation_unary;
	immediate_operation_t operation_imm;
	register_operation_t operation_reg;
	label_t label;
	go_instruction_t instruction_go;
	test_register_instruction_t instruction_test_reg;
	test_immediate_instruction_t instruction_test_imm;
	push_register_instruction_t instruction_push_reg;
	pop_register_instruction_t instruction_pop_reg;
	print_register_instruction_t instruction_print_reg;
	load_save_instruction_t instruction_load_save;
}

// The '@' of the label's beginning
%token label_decl

// The '?' (test operator)
%token test_decl

// The push's beginning
%token push_decl

// The pop's beginning
%token pop_decl

// Push and pop
%token push
%token pop

// Raw instruction
%token <cmd> cmd

// A go instruction
%token <go> go
%token <go_id> go_id

%token <operand> operand
%token <unary_operand> unary_operand
%token <value> constant
%token <reg> gp_register
%token <identifier> identifier
%token <print_format> print_decl
%token <load_save_token> load_save_token

%type <operation_unary> unary_operation
%type <operation_imm> immediate_operation
%type <operation_reg> register_operation
%type <label> label_type
%type <instruction_go> go_instruction
%type <instruction_test_reg> test_register_instruction
%type <instruction_test_imm> test_immediate_instruction
%type <instruction_push_reg> push_register_instruction
%type <instruction_pop_reg> pop_register_instruction
%type <instruction_print_reg> print_register_instruction
%type <instruction_load_save> load_save_instruction

// The root of the abstract syntax tree (AST) is named program
%start program

// Priorities
%right '\n'

%%
program:
	| program '\n' ;
	| program cmd								{ genInstruction($2); }
	| program unary_operation					{ genUnaryOperation($2); }
	| program immediate_operation				{ genImmediateOperation($2); }
	| program register_operation				{ genRegisterOperation($2); }
	| program label_type						{ registerLabel($2); }
	| program go_instruction					{ genGo($2); }
	| program test_register_instruction			{ genTestRegister($2); }
	| program test_immediate_instruction		{ genTestImmediate($2); }
	| program push_register_instruction			{ genPushRegister($2); }
	| program pop_register_instruction			{ genPopRegister($2); }
	| program print_register_instruction		{ genPrintRegister($2); }
	| program load_save_instruction				{ genLoadSaveInstruction($2); }
	| program print_decl '\n'					{ genInstruction(PRINT_NL); }
	;

unary_operation: gp_register unary_operand '\n'						{ $$.reg = $1; $$.id = $2; };

immediate_operation: gp_register operand constant '\n'				{ $$.reg = $1; $$.operand = $2; $$.constant = $3; };

register_operation: gp_register operand gp_register '\n'			{ $$.target = $1; $$.operand = $2; $$.value = $3; };

label_type: label_decl identifier '\n'								{ $$.id = $2; };

go_instruction: identifier go_id '\n'								{ $$.labelId = $1; $$.goId = $2; };

test_register_instruction: gp_register test_decl gp_register '\n'	{ $$.first = $1; $$.second = $3; };

test_immediate_instruction: gp_register test_decl constant '\n'		{ $$.first = $1; $$.second = $3; };

push_register_instruction: push_decl gp_register '\n'				{ $$.reg = $2; };

pop_register_instruction: pop_decl gp_register '\n'					{ $$.reg = $2; };

print_register_instruction: print_decl gp_register '\n'				{ $$.format = $1; $$.reg = $2; };

load_save_instruction: gp_register load_save_token constant			{ $$.reg = $1; $$.type = $2; $$.offset = $3; };
%%
