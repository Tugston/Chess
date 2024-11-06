#include "Window.h"

#include<GL/glew.h>
#include<SDL.h>

#include<iostream>

using namespace GraphicsEngine;

Window::Window() : Window(800.f){}

Window::Window(const float& windowSize)
{
	this->window = SDL_CreateWindow("Chess", 200, 200, windowSize, windowSize, SDL_WINDOW_OPENGL);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Could not initialize SDL";
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);


	SDL_GL_CreateContext(this->window);
	glewInit();
}

Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
	delete window;
}
