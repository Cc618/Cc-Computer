# Formats a binary string to a C-style array.

# Arguments : <input> [output]
# - input : The input binary file 
# - output : The output C-style file
# If no output, it prints the code to stdout

import sys


# Params #
# The number of bytes per line
line_size = 8


# Check arguments
if len(sys.argv) != 2 and len(sys.argv) != 3:
    print("Error : Invalid argument count\nArguments : <input> [output]")
    sys.exit(-1)


# The code content
code = """#pragma once

// !!! This file is auto-generated by ccasm converter
// !!! DON'T modify this file, the data can be lost

#include <cinttypes>

// The program's data
static const uint8_t PROGRAM_DATA[] = {"""


with open(sys.argv[1], "rb") as f:
    i = 0

    byte = f.read(1)
    while byte:
        if i != 0:
            code += ", "

        if i % line_size == 0:
            code += '\n\t'

        code += "0x{:02X}".format(int.from_bytes(byte, "little"))

        byte = f.read(1)

        i += 1


code += "\n};\n"


if len(sys.argv) == 3:
	with open(sys.argv[2], "w") as f:
		f.write(code)
else:
	print(code)
