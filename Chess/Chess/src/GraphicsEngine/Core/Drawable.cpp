#include "Drawable.h"

#include "GL/glew.h"
#include "Macros.h"

GraphicsEngine::Drawable::Drawable()
{
}

GraphicsEngine::Drawable::~Drawable()
{
	delete shader;
}
