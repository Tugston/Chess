#pragma once
#define GLM_ENABLE_EXPERIMENTAL

//libs
#include<SDL.h>
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<gtx/string_cast.hpp>
#include<stb_image.h>

//std
#include<iostream>
#include<vector>

//_________GAME INCLUDES________
#include "../Game/Board.h"

//_________CORE INCLUDES________
#include "../GraphicsEngine/Core/Window.h"
#include "../GraphicsEngine/Core/Shader.h"
#include "../GraphicsEngine/Core/Drawable.h"

//________Component INCLUDES_________
#include "../GraphicsEngine/Components/Camera.h"


//game class that handles everything
namespace Chess {
	class Game {
	public:

	public:
		Game();
		~Game();

		void Start();
	private:
		void Draw(Uint32 DeltaTime);
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

		glm::mat4 model = glm::mat4(1.f);


	private:

		//CORE
		GraphicsEngine::Window* window;
		GraphicsEngine::Shader* shader;

		//components
		GraphicsEngine::Camera* camera;

		Chess::Board* board;

		std::vector<GraphicsEngine::Drawable*> drawables;


		bool quit;

		Uint32 lastUpdateTime;
	};
}