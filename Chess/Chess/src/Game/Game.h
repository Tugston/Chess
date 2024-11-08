#pragma once


#define GLM_ENABLE_EXPERIMENTAL

//libs
#include<SDL.h>
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<gtx/string_cast.hpp>

//std
#include<iostream>
#include<vector>
#include<map>

//_________GAME INCLUDES________
#include "../Game/Board.h"


//_________CORE INCLUDES________
#include "../GraphicsEngine/Core/Window.h"
#include "../GraphicsEngine/Core/Shader.h"
#include "../GraphicsEngine/Core/Drawable.h"
#include "../GraphicsEngine/Core/SpriteRenderer.h"

//________Component INCLUDES_________
#include "../GraphicsEngine/Components/Camera.h"


//game class that handles everything
namespace Chess {

	class BasePiece;

	class Game {
	public:

	public:
		Game();
		~Game();

		void Start();
	private:
		void Draw(Uint32 DeltaTime);
		void Tick();

	
		void SetupPieces();

	private:

		//CORE
		GraphicsEngine::Window* window;

		//components
		GraphicsEngine::Camera* camera;

		Chess::Board* board;
		GraphicsEngine::SpriteRenderer* spriteRenderer;

		std::vector<GraphicsEngine::Drawable*> drawables;


		std::vector<Chess::BasePiece*> WhitePieces;
		std::vector<Chess::BasePiece*> BlackPieces;


		bool quit;

		Uint32 lastUpdateTime;

	};
}