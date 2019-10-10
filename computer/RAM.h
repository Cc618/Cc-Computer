#pragma once

// All classes that handles data

#include "properties.h"


class RAM
{
	RAM();

	char* data[RAM_SIZE];
};


class Program
{
	Program();

	char* data;
};


