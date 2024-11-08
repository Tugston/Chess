#include "Game.h"

#include "../GraphicsEngine/Core/Macros.h"

//pieces
#include "Pieces/King.h"
#include "Pieces/Bishop.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"


using namespace Chess;

Game::Game() : quit(false), lastUpdateTime(0){

	//lightweight gl classes are created on constructor
	window = new GraphicsEngine::Window;
	camera = new GraphicsEngine::Camera;


	board = nullptr;
	spriteRenderer = nullptr;


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
	//in order to be drawn easier
	board = new Board;
	drawables.push_back(board);

	spriteRenderer = new GraphicsEngine::SpriteRenderer;
	SetupPieces();
	

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
	//location offsets
	float currentXOffset = 0;
	float currentYOffset = 0;




	//populate white pieces
	//could loop this, but whats the point of using the enum here if I just pass uints anyways
	WhitePieces.push_back(new Rook(WHITE, glm::vec2(0, 0)));
	WhitePieces.push_back(new Knight(WHITE, glm::vec2(1, 0)));
	WhitePieces.push_back(new Bishop(WHITE, glm::vec2(2, 0)));
	WhitePieces.push_back(new Queen(WHITE, glm::vec2(3, 0)));
	WhitePieces.push_back(new King(WHITE, glm::vec2(4, 0)));
	WhitePieces.push_back(new Bishop(WHITE, glm::vec2(5, 0)));
	WhitePieces.push_back(new Knight(WHITE, glm::vec2(6, 0)));
	WhitePieces.push_back(new Rook(WHITE, glm::vec2(7, 0)));

	//we can loop the pawns cause I am tired of copy pasting
	for (int i = 7; i >= 0; i--)
	{
		WhitePieces.push_back(new Pawn(WHITE, glm::vec2(i, 1)));
	}

	BlackPieces.push_back(new Rook(BLACK, glm::vec2(0, 7)));
	BlackPieces.push_back(new Knight(BLACK, glm::vec2(1, 7)));
	BlackPieces.push_back(new Bishop(BLACK, glm::vec2(2, 7)));
	BlackPieces.push_back(new Queen(BLACK, glm::vec2(3, 7)));
	BlackPieces.push_back(new King(BLACK, glm::vec2(4, 7)));
	BlackPieces.push_back(new Bishop(BLACK, glm::vec2(5, 7)));
	BlackPieces.push_back(new Knight(BLACK, glm::vec2(6, 7)));
	BlackPieces.push_back(new Rook(BLACK, glm::vec2(7, 7))); 

	for (int i = 7; i >= 0; i--)
	{
		BlackPieces.push_back(new Pawn(BLACK, glm::vec2(i, 6)));
	} 

	for (int i = 0; i < WhitePieces.size(); i++)
	{
		spriteRenderer->AddSpriteData(WhitePieces.at(i)->GetOffset());
		spriteRenderer->AddSpriteID(WhitePieces.at(i)->GetTextureID());
	}

	for (int i = 0; i < BlackPieces.size(); i++)
	{
		spriteRenderer->AddSpriteData(BlackPieces.at(i)->GetOffset());
		spriteRenderer->AddSpriteID(BlackPieces.at(i)->GetTextureID());
	}


	spriteRenderer->SetupBuffer();
	drawables.push_back(spriteRenderer);
}
