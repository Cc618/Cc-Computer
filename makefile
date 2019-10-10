
all: assembler/makefile computer/makefile
	cd computer && make
	cd assembler && make


clean: assembler/makefile computer/makefile
	cd computer && make clean
	cd assembler && make clean
	rm -rf bin/*


example-abc: bin/ccasm bin/cc_computer
	bin/ccasm examples/abc.ccasm bin/code.bin && bin/cc_computer bin/code.bin


example-fib: bin/ccasm bin/cc_computer
	bin/ccasm examples/fibonacci.ccasm bin/code.bin && bin/cc_computer bin/code.bin
