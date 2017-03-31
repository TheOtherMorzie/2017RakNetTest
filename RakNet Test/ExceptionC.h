#pragma once

#include <exception>

struct ExceptionC
{
	ExceptionC(char* e, int l, char* f) { error = e; line = l; file = f; };

	char* error;
	int line;
	char* file;
};

#define THROWC(e)\
throw ExceptionC(e, __LINE__, __FILE__);

