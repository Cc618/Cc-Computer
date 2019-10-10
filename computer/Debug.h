#pragma once

#include <cinttypes>
#include <iostream>

// To debug

namespace debug
{
	static inline void log(const char *s)
	{
#ifndef CC_NO_DEBUG
		std::cout << s << '\n';
#endif
	}

	static inline void logByte(const uint8_t b)
	{
#ifndef CC_NO_DEBUG
		std::cout << "Byte : " << b << '\n';
#endif
	}

	static inline void logWord(const uint16_t w)
	{
#ifndef CC_NO_DEBUG
		std::cout << "Word : " << w << '\n';
#endif
	}

	static inline void logRegisters(const Computer &pc)
	{
#ifndef CC_NO_DEBUG
		// Test Flags //
		// If all flags are on, this text is displayed
		static const char* FLAGS_TEXT = "=><?????";

		std::string flags = FLAGS_TEXT;

		// Iterate through flags to set the text
		for (int i = 0; i < 8; ++i)
			// Set the char for this flag to . (off)
			if (!(pc.reg.f & (1 << i)))
				flags[i] = '.';


		// Debug //
		printf(
			"--- Registers ---\n"
			"- General Purpose -\n"
			"x : %04hx | %hd\n"
			"y : %04hx | %hd\n"
			"- Pointers -\n"
			"s : %04hx | %hd\n"
			"r : %04hx | %hd\n"
			"- Program Counter -\n"
			"c : %04hx | %hd\n"
			"- Flags (f) -\n"
			"f : %s\n"
			,
			pc.reg.x, pc.reg.x,
			pc.reg.y, pc.reg.y,
			pc.reg.s, pc.reg.s,
			pc.reg.r, pc.reg.r,
			pc.reg.c, pc.reg.c,
			flags.c_str()
		);
#endif
	}

	// LINES is the number of lines (8 bytes) to display 
	static inline void logStack(const size_t LINES, const Computer &PC)
	{
#ifndef CC_NO_DEBUG
		puts("--- Memory ---");

		// For each line
		for (size_t i = 0; i < LINES; ++i)
			printf(
				"%02hhx %02hhx %02hhx %02hhx  %02hhx %02hhx %02hhx %02hhx\n",
				PC.ram.data[(i << 3) + 0],
				PC.ram.data[(i << 3) + 1],
				PC.ram.data[(i << 3) + 2],
				PC.ram.data[(i << 3) + 3],
				PC.ram.data[(i << 3) + 4],
				PC.ram.data[(i << 3) + 5],
				PC.ram.data[(i << 3) + 6],
				PC.ram.data[(i << 3) + 7]
			);
#endif
	}
};

