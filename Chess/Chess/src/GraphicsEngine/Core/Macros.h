#pragma once

#include<GL/glew.h>
#include<iostream>

//vvvvvvvvvvvvvvvvvv Error Handling Routine vvvvvvvvvvvvvvvvvvvvvv
static void GLClearAllErrors() {
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

static bool GLCheckErrorStatus(int line, std::string file) {
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL Error: " << error << "\tLine: " << line << "\tFile: " << file << std::endl;
		return true;
	}
	return false;
}

#define glCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(__LINE__, __FILE__);

//^^^^^^^^^^^^^^^^^^ Error Handling Routine ^^^^^^^^^^^^^^^^^^^^^^
