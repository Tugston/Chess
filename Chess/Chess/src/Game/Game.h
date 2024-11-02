#pragma once

#include<SDL.h>
#include<GL/glew.h>
#include<stb_image.h>

#include<iostream>
#include<vector>

#include "../GraphicsEngine/Core/Window.h"
#include "../GraphicsEngine/Core/Shader.h"

//game class that handles everything
namespace Chess {
	class Game {
	public:

	public:
		Game();
		~Game();

		void Start();
	private:
		void Draw();
		void Tick();

		std::vector<float> vertexData{
			 0.5f,  0.5f, 0.f,
			 0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f,
			-0.5f,  0.5f, 0.f
		};

		std::vector<unsigned int> indices{
			0, 1, 3,
			1, 2, 3
		};

		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;


	private:
		GraphicsEngine::Window* window;
		GraphicsEngine::Shader* shader;
		bool quit;
	};
}