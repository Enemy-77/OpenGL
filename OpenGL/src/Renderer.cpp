#include "Renderer.h"
#include <iostream>
#include <gl/glew.h>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// second  GLCAll is better, more clear.
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum  error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "):" << function << " "
			<< file << ":" << line << std::endl;
		return false;
	}
	return true;
}