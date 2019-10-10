#pragma once

#ifdef __cplusplus
#	define cfunc extern "C"
#else
#	define cfunc
#endif

extern char semanticError; 

cfunc void yyerror(const char *s);
cfunc void yyerrorRaw(const char *s);
cfunc void yyerrorInvalidChar(const char c);
