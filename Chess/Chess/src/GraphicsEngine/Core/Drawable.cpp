#include "Drawable.h"

#include "GL/glew.h"
#include "Macros.h"

GraphicsEngine::Drawable::Drawable()
{
	//gen buffers
	glCheck(glGenVertexArrays(1, &vao));
	glCheck(glGenBuffers(1, &vbo));
	glCheck(glGenBuffers(1, &ebo));

	//ensure nothing is bound after leaving this constructor
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GraphicsEngine::Drawable::~Drawable()
{
	delete shader;
}
