/* Read only 1 input file */
%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"
#include "main.h"
#include "../shared/raw_opcode.h"


extern void yyerror(const char *s);

void make_go(go_id_t *id, const char *text);
void make_print_format(print_format_t *format, const char *text);
%}


/* Whitespaces */
EMPTY [ \t]

/* Constant values */
NUMBER [0-9]+

/* Registers */
GP_REGISTER [xy]

/* Operators */
OPERATOR ([=])|([\+\*-]=)
UNARY_OPERATOR (\+\+)|(--)

/* Comments */
COMMENT [#].*[\n]?

/* Special chars */
SPECIAL [\n]

/* Names */
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

/* Go */
GO_ID \((@|=|<|>|!|(<=)|(>=))?\)

/* Print */
PRINT_DECL $[c]?

/* Save or Load */
SAVE_LOAD [:][<>]

%%
{EMPTY}				;																// Ignore whitespaces
{COMMENT}			;
{NUMBER}			{ yylval.value = atoi(yytext); return constant; }
{GP_REGISTER}		{ yylval.reg.id = yytext[0]; return gp_register; }
{OPERATOR}			{ yylval.operand = yytext[0]; return operand; }
{SAVE_LOAD}			{ yylval.load_save_token = yytext[1]; return load_save_token; }
{UNARY_OPERATOR}	{ yylval.unary_operand = yytext[0]; return unary_operand; }
{GO_ID}				{ make_go(&yylval.go_id, yytext); return go_id; }
{PRINT_DECL}		{ make_print_format(&yylval.print_format, yytext); return print_decl; }
{SPECIAL}			{ return yytext[0]; }

"exit"				{ yylval.cmd = EXIT; return cmd; }
"trap"				{ yylval.cmd = TRAP; return cmd; }
"leave"				{ yylval.cmd = LEAVE; return cmd; }
"push"				{ return push; }
"pop"				{ return pop; }

{IDENTIFIER}		{ yylval.identifier = strdup(yytext); return identifier; }

"@"					{ return label_decl; }
"?"					{ return test_decl; }
">"					{ return push_decl; }
"<"					{ return pop_decl; }

.					{ yyerrorInvalidChar(yytext[0]); }
%%


// Generates a go id
void make_go(go_id_t *id, const char *text)
{
	const size_t LENGTH = strlen(text);

	if (LENGTH == 2)
		id->first = '.';
	else if (LENGTH == 3)
	{
		id->first = text[1];
		id->second = '.';
	}
	else
	{
		id->first = text[1];
		id->second = text[2];
	}
}

// Generates a print format
void make_print_format(print_format_t *format, const char *text)
{
	const size_t LENGTH = strlen(text);

	if (LENGTH == 1)
		*format = 'i';
	else
		*format = yytext[1];
}
