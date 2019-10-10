# Cc-Computer
A computer simulator with a custom assembler which compiles programs on it

## Usage
### Build
#### Depencies
- gcc
- g++
- flex
- bison

#### Make
To build computer and assembler, go to the main directory :
```bash
$ make
```
You can also make only the computer or the assembler project by executing
make in each project directory.  

The programs are located in the bin folder.

### Example
Once builded, you can execute an example :
```bash
$ make example-fib
(...)
1
1
2
3
5
8
13
(...)
```
Another example is example-abc. You can also execute other examples in the directory
[examples](examples "Examples directory").

### Assembler
To compile an assembler source file :
```bash
$ ./ccasm <input> <output> 
```

### Computer
To execute the binary code compiled by the assembler
```bash
$ ./cc_computer <code_path>
```

## Assembler
### A small example
```python
@ main
	cc()
leave

@ cc
	# Ascii code for 'A' 
	y = 65

	@ loop_start
		# Print y as char
		$c y

		# Increase y
		y++

		# Continue if y < 91
		y ? 91
		loop_start(<)

	# New line
	$
leave
```

Output :
```bash
ABCDEFGHIJKLMNOPQRSTUVWXYZ
--- Return ---
Word : 0
```

[This example more detailed](examples/abc.ccasm "abs.ccasm in examples").  

To see in depth how the syntax works see
[Assembler syntax's guide](examples/syntax.md "syntax.md in examples").


## Computer
- 16 bit
- Little Endian (0x1234 is stored 0x34 0x12 in memory)
- Integer only (no floating point calculus)
- Jumps are absolute, not relative (GO ADDRESS not GO OFFSET)


## Project Architecture
- [computer](computer) : The source for the computer simulator (cc_computer)
- [assembler](assembler) : The source for the assembler language (ccasm)
- [shared](shared) : Files used for computer and assembler folders
- [examples](examples) : Examples for the assembler
- [utils](utils) : A program to convert a binary string to C-style array.
