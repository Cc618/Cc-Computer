#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "code_generator.h"


/////// TODO: Warning without exit at end or auto add


// Flex functions or variables
int yyparse();
void yyerror(const char *s);

extern FILE *yyin;
extern int yylineno;

// 1 if we encounter an error outside of the lexer and parser
char semanticError = 0;


int main(int argc, const char **argv)
{
	//// Arguments ////
	if (argc != 3)
	{
		// Help
		if (argc == 1)
		{
			printf(
				"Usage : ccasm <input> <output>\n"
				"Cc Assembler (ccasm) is an assembler language compiler "
				"targeting Cc Computer's instruction set.\n"
			);

			return 0;
		}

		// Error
		fprintf(stderr, "Usage : ccasm <input> <output>\n");

		return -1;
	}


	// Set the stream
	yyin = fopen(argv[1], "r");


	// Parse code
	int ret = yyparse();

	// There is an error, set the return value
	if (semanticError)
		ret = -1;


	// No errors
	if (ret == 0)
		// Last pass to remap go statements...
		ret = finalizeCode();

	// If there still is no error
	if (ret == 0)
		generateFile(argv[2]);

	return ret;
}


void yyerror(const char *s)
{
	fprintf(stderr, "Error @ %i: %s\n", yylineno, s);
}

void yyerrorRaw(const char *s)
{
	fprintf(stderr, "Error : %s\n", s);
}

void yyerrorInvalidChar(const char c)
{
	fprintf(stderr, "Error @ %i: Invalid char '%c'\n", yylineno, c);
}
