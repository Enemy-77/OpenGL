#pragma once

#include <string>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

// second  GLCAll is better, more clear.
bool GLLogCall(const char* function, const char* file, int line);