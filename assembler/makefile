
all: lexer.l parser.y main.c code_generator.cpp ../bin
	flex lexer.l
	bison -d parser.y

	gcc -c main.c lex.yy.c parser.tab.c
	g++ -o ../bin/ccasm code_generator.cpp *.o


clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c *.o


../bin:
	mkdir ../bin
