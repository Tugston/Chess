#include "Game.h"

#include "../GraphicsEngine/Core/Macros.h"


using namespace Chess;

Game::Game() : quit(false){

	//lightweight gl classes are created on constructor
	window = new GraphicsEngine::Window;
	camera = new GraphicsEngine::Camera;

}

Game::~Game()
{
	for (int i = 0; i < drawables.size(); i++)
	{
		delete drawables[i];
	}

	drawables.clear();

	delete board;
	delete spriteRenderer;
	delete window;
}

void Game::Start()
{

	//heavier using gl classes created on start
	
	//board has gameplay material, so need a direct pointer as well as pushing it into the drawables
	//in order to be drawn
	board = new Board;
	drawables.push_back(board);

	spriteRenderer = new GraphicsEngine::SpriteRenderer;

	

	Tick();
}

void Game::Draw(Uint32 DeltaTime)
{

	glClearColor(.48f, .85f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < drawables.size(); i++)
	{

		//can't bind shader in draw call in the class because I have to set the uniforms here,
		//and it needs camera data to properly configure them and I do not want to pass the camera
		//to the base drawable just for this
		drawables.at(i)->GetShader()->Use();

		drawables.at(i)->GetShader()->SetUniformMat4(camera->GetViewMatrix(), "view");
		drawables.at(i)->GetShader()->SetUniformMat4(camera->projection, "projection");
		drawables.at(i)->GetShader()->SetUniformMat4(drawables.at(i)->GetModelMatrix(), "model");

		drawables.at(i)->Draw();
	}


	SDL_GL_SwapWindow(window->GetWindow());
	
	
}

void Game::Tick()
{
	while (!quit)
	{
		Uint32 currentTime = SDL_GetTicks();
		Uint32 DeltaTime = currentTime - lastUpdateTime;


		SDL_Event keyEvent;
		while (SDL_PollEvent(&keyEvent))
		{
			switch (keyEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
			}
		}


		Draw(DeltaTime);

		if (DeltaTime >= 1)
			lastUpdateTime = currentTime;
	}

	
}

void Chess::Game::SetupPieces()
{
	float currentXOffset = 0;
	float currentYOffset = 0;

	//add white piece data
	for (int i = 0; i < 16; i++) {
		if (i == 8) {
			currentYOffset += 1;
			currentXOffset = 0;
		}
		spriteRenderer->AddSpriteData(glm::vec2(currentXOffset, currentYOffset));
		currentXOffset += 1;
	}

	currentXOffset = 0;
	currentYOffset = 6;

	//add black piece data
	for (int i = 0; i < 16; i++)
	{
		if (i == 8) {
			currentYOffset += 1;
			currentXOffset = 0;
		}
		spriteRenderer->AddSpriteData(glm::vec2(currentXOffset, currentYOffset));
		currentXOffset += 1;
	}


	spriteRenderer->SetupBuffer();
	drawables.push_back(spriteRenderer);
}
